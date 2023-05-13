# Do not edit this script !!!
file="$1"
if test ! -f $1
then exit 1
else
output=$(gcc -Wall -Wextra "$file" -o /dev/null 2>&1)
errors=$(echo "$output" | grep -c 'error')
warns=$(echo "$output" | grep -c 'warning')
echo "$errors"
echo "$warns"
fi 