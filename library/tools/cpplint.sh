find ../include/ ../src/ ../test/ ../../simulator/m2etis/src/ \( -name \*.cpp -o -name \*.h \) -exec python cpplint.py --counting=detailed --filter=-whitespace/line_length,-whitespace/tab,-build/header_guard,-build/include_what_you_use,-runtime/int {} + 2> lint.checks
sed -i '/Category .* errors found/d' lint.checks
RESULT=`wc -l lint.checks`
NUMBER=${RESULT//[a-z\.]/}

echo $NUMBER

if [ $NUMBER == 0 ]; then
	exit 0;
else
	exit 1;
fi
