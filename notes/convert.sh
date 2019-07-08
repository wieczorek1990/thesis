for file in svg/*.svg
do
  basename=`basename $file`
  filename=${basename%.*}
  out=png/svg/$filename.png
  inkscape -z -d 1200 -e $out $file
done
