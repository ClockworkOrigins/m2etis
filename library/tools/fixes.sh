find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's/[ \t]*$//' {} \; # remove trailing whitespace
find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's/,\([^ ]\)/, \1/g' {} \; # comma space
find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's#//\([^ w]\)#// \1#g' {} \; # comment space
find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's/( /(/g' {} \; # '( '
find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's/\([^\t ]\){/\1 {/g' {} \; # 'a{'
#find ../ \( -name \*.cpp -o -name \*.h \) -exec sed -i 's/(+|-|*|/|=|<|>//g' {} \; # KA
