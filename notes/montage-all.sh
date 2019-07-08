DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
groups='mouse tablet'
notes='whole half quarter eighth sixteenth thirty-second sharp flat natural dot'
for group in $groups
do
  for note in $notes
  do
    $DIR/montage.sh "png/$group/$note" "montaged/$group-$note"
  done
done
$DIR/montage.sh png/svg montaged/svg
