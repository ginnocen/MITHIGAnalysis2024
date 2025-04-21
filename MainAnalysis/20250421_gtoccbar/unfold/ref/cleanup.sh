grep -q Verkerke $1
if [ $? -eq 0 ]; then
    sed -i '1,5d' $1
fi
