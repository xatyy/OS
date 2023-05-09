file="$1"

if test ! -f $1
then echo "Not a regular file"
exit 1
else
output=$(gcc -Wall -Wextra "$file" 2>&1)
errors=$(echo "$output" | grep -c 'error')
warns=$(echo "$output" | grep -c 'warning')

echo "$errors"
echo "$warns"
fi 