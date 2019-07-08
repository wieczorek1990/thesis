DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
groups='mouse tablet'
notes='whole half quarter eighth sixteenth thirty-second sharp flat natural dot'
for group in $groups
do
  for note in $notes
  do
    $DIR/resize.sh "montaged/$group-$note.png" "resized/$group-$note.png"
  done
done
$DIR/resize.sh "montaged/svg.png" "resized/svg.png"
