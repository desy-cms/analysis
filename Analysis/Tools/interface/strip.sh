
sed 's/]],/&\n/g' $1  > temp; sed -e 's/{//g' -e 's/}//g' -e 's/"//g' -e 's/,/ /g' -e 's/]/ /g' -e 's/\[/ /g' -e 's/:/ /g'   temp > t ; mv t temp
sed -i 's/^[ \t]*//' temp
sed -i '/^$/d' temp
#cat temp


