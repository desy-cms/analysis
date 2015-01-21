#! /usr/bin/env python

import hashlib

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.SequenceTypes as _detail

verbose = False

def clone_module(module, *changes):
    """Clone a module applying a list of changes.

    'changes' is expected to be one or more strings of the form '[pset.]parameter = cms.type(value)'

    Note that the cloned module is not inserted in the process, and is thus unnamed.
    """
    cloned = module.clone()

    for change in changes:
        exec ('cloned.' + change)
    return cloned


def clone_module_as(process, module, name, *changes):
    """Clone a module applying a list of changes, and insert it in the process with a new name.

    'changes' is expected to be one or more strings of the form '[pset.]parameter = cms.type(value)'
    """
    cloned = clone_module(module, *changes)
    process.__setattr__(name, cloned)
    return cloned


def list_all_modules(sequence):
    """Return the orered list all modules in a sequence or path"""

    # based on FWCore.ParameterSet.SequenceTypes.ModuleNodeVisitor and NodeNameVisitor
    class ModuleNameVisitor(object):
        def __init__(self,l):
            self.l = l
        def enter(self,visitee):
            if isinstance(visitee, _detail._SequenceLeaf):
                self.l.append(visitee.label_())
        def leave(self,visitee):
            pass
    result = []
    visitor = ModuleNameVisitor(result)
    sequence.visit(visitor)
    return result


def list_all_sequences(sequence):
    """Retun the ordered list all sequences in a sequence or path"""

    # based on FWCore.ParameterSet.SequenceTypes.SequenceVisitor and NodeNameVisitor
    class SequenceNameVisitor(object):
        def __init__(self,l):
            self.l = l
        def enter(self,visitee):
            if isinstance(visitee, cms.Sequence):
                self.l.append(visitee.label_())
        def leave(self,visitee):
            pass
    result = []
    visitor = SequenceNameVisitor(result)
    sequence.visit(visitor)
    return result


def list_all_inputs(module):
    """Return all InputTags referenced by a module or pset, along with their values"""
    inputs = {}
    for name in module.parameterNames_():
        attribute = module.__dict__[name]
        if isinstance(attribute, cms.InputTag):
            # add the InputTag to the list
            inputs[name] = attribute.value()
        elif isinstance(attribute, cms.VInputTag):
            # add each InputTag to the list
            for i,val in enumerate(attribute.value()):
                inputs['%s[%d]' % (name, i)] = val
        elif isinstance(attribute, cms.PSet):
            # look for InputTags in the PSet
            for key, val in list_all_inputs( attribute.value() ).iteritems():
                inputs['%s.%s' % (name, key)] = val
        elif isinstance(attribute, cms.VPSet):
            # look for InputTags in each PSet
            for i,pset in enumerate(attribute.value()):
                for key, val in list_all_inputs( pset ).iteritems():
                    inputs['%s[%d].%s' % (name, i, key)] = val
    return inputs

#FIXME there should be a better way than this
def get_inputtag_str(obj):
    if type(obj) == str:
        return obj
    else:
        return obj.getModuleLabel()



def find_affected_modules(sequence, changed):
    """Find all modules of the given sequence or path, affected by cloning/renaming a set of existing modules"""

    # list all modules in the sequence or path
    # Note Bene: this does not work with unschedule execution !
    modules = list_all_modules(sequence)
    # find the InputTags referenced by each module
    # FIXME possible optimisation: extract the inputs once for all modules, instead of re-doing it within each sequence
    class ExtractInputTagVisitor(object):
        def __init__(self, inputs):
            self.inputs = inputs

        def enter(self, visitee):
            if isinstance(visitee, _detail._SequenceLeaf):
                label = visitee.label_()
                self.inputs[label] = set( get_inputtag_str(inputtag).split(':')[0] for inputtag in list_all_inputs( visitee ).values() )

        def leave(self, visitee):
            pass

    inputs = {}
    sequence.visit( ExtractInputTagVisitor(inputs) )
   
    # assumes that the modules are in the correct order, that is, a module will not reference any module *following* it.
    # keep the list of affected modules in order as they appear in the path orsequence.
    affected = []
    for label in modules:
        if label in affected:
            # we already know that this module has to be cloned
            continue
        for candidate in set(changed) | set(affected):
            if candidate in inputs[label]:
                # this module is affected by the original clonig/renaming, or by a knock-off
                affected.append(label)
                break
    # add any 'changed' module that is not in the list of 'affected' modules to the beginning of the list
    return [ label for label in changed if label not in affected ] + affected


def find_sequences_with_modules(sequence, labels):
    """Find all sequences in the given sequence or path, containing any of the given modules"""

    class FindSequenceWithModulesVisitor(object):
        def __init__(self, sequences, labels):
            self.sequences = sequences
            self.labels    = labels
            self.visiting  = set()

        def enter(self, visitee):
            if isinstance(visitee, cms.Sequence):
                label = visitee.label_()
                self.visiting.add(label)
            elif isinstance(visitee, _detail._SequenceLeaf):
                label = visitee.label_()
                if label in self.labels:
                    for s in self.visiting:
                        self.sequences.setdefault(s, []).append(label)

        def leave(self,visitee):
            if isinstance(visitee, cms.Sequence):
                label = visitee.label_()
                self.visiting.remove(label)

    sequences = dict()
    sequence.visit(FindSequenceWithModulesVisitor(sequences, labels))
    return sequences


def clone_path(process, pathname, process_changes, rename, verbose=False):
    """Clone a path, apply a list of changes, and add the path to the process with a new name.

    'process_changes' is expected to be one or more strings of the form 'process.module[.pset].parameter = cms.type(value)'
    """
    # extract, from the list of process_changes, the list of modules and the changes for each module
    changes = {}
    for element in process_changes:
        # extract 'process', the module label, and the customisation from each change
        try:
            (_, label, change) = element.split('.', 2)
            assert _ == 'process'
        except:
            raise ValueError("ill formed customisation '%s'" % element)
        # insert an empty list for a new module, and remember the change
        changes.setdefault(label, []).append( change )

    # dictionary associating the labels of the original modules and sequences to the labels of the clones
    clones = {}

    # use sorted to extract a list from the mapping, with a consistent order;
    # it is important that find_affected_modules() returns the modules in the order in which they appear in the path,
    # so that they can be update with a single pass

    #need to map sequence string to sequence object
    path= process.__dict__[pathname] 
    if verbose: print "Base path for cloning:", pathname
    affected_modules = find_affected_modules(path, sorted(changes))
    #print "@@ affected modules:", affected_modules
    for label in affected_modules:
        module = process.__dict__[label]
        # make sure all affected modules can receive changes
        changes.setdefault(label, [])
        # additional changes to update the InputTags
        inputs = list_all_inputs(module)
        for attribute, original_inputtag in inputs.iteritems():
            original_label = original_inputtag.split(':')[0]
            if original_label in affected_modules:
                if original_label not in clones:
                    raise RuntimeError('wrong ordering of modules to be cloned\nThis should never happen.')
                # compute the new InputTag replacing the label, but keeping the original instance and process (if set)
                new_inputtag = ':'.join([ clones[original_label] ] + original_inputtag.split(':')[1:])
                changes[label].append('%s = "%s"' % (attribute, new_inputtag))                
        # once all changes are know, define a unique hash from the list of changes
        #print 'label: ', label
        if label.startswith("hltPre"):
            clones[label] = label + rename
        else :    
            hash = hashlib.md5( '\n'.join(sorted(changes[label])) ).hexdigest().capitalize()
            clones[label] = label + hash
        # if a clone with this hash does not exist, create it
        if clones[label] not in process.__dict__:
            if verbose:
                print 'process.%s = %s.clone()' % (clones[label], label)
                for change in changes[label]:
                    print 'process.%s.%s' % (clones[label], change)
                print
            clone_module_as(process, module, clones[label], *changes[label])

    # find the Sequences that need to be cloned
    affected_sequences = find_sequences_with_modules(path, affected_modules)
    for sequence_label, labels in affected_sequences.iteritems():
        # for each sequence to be cloned, define a hash from the names of the cloned modules it will contain
        hash = hashlib.md5( '\n'.join(clones[label] for label in labels) ).hexdigest().capitalize()
        clones[sequence_label] = sequence_label + hash
    # clone all sequences for which the clone does not already exist, and insert them in the process
    for sequence_label in affected_sequences:
        sequence = process.__dict__[sequence_label]
        if clones[sequence_label] not in process.__dict__:
            cloned_sequence = sequence.copy()
            process.__setattr__(clones[sequence_label], cloned_sequence)

    # update the cloned sequences replacing the original modules and sequences with the cloned ones
    original_item = dict( (label, process.__dict__[label]        ) for label in clones )
    cloned_item   = dict( (label, process.__dict__[clones[label]]) for label in clones )
    for sequence_label in affected_sequences:
        cloned_sequence = cloned_item[sequence_label]
        if verbose:
            print 'process.%s = process.%s.copy()' % (clones[sequence_label], sequence_label)
            for label in clones:
                if cloned_sequence.replace( original_item[label], cloned_item[label] ):
                    print 'process.%s.replace( process.%s, process.%s )' % (clones[sequence_label], label, clones[label])
            print
        else:
            for label in clones:
                cloned_sequence.replace( original_item[label], cloned_item[label] )

    # clone the path, and update it to use the cloned modules and sequences
    cloned_path = path.copy()
    if verbose:
        print 'process.%s_clone = process.%s.copy()' % (path.label_(), path.label_())
        for label in clones:
            if cloned_path.replace( original_item[label], cloned_item[label] ):
                print 'process.%s_clone.replace( process.%s, process.%s )' % (path.label_(), label, clones[label])
    else:
        for label in clones:
            cloned_path.replace( original_item[label], cloned_item[label] )
    return cloned_path

