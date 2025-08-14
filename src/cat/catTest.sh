main() {

    local flags=("b" "e" "n" "s" "t" "E" "T" 
    "be" "bn" "bs" "bt" "bE" "bT" 
    "eb" "en" "es" "et" "eE" "eT" 
    "nb" "ne" "ns" "nt" "nE" "nT" 
    "sb" "se" "sn" "st" "sE" "sT" 
    "tb" "te" "tn" "ts" "tE" "tT" 
    "Eb" "Ee" "En" "Es" "Et" "ET" 
    "Tb" "Te" "Tn" "Ts" "Tt" "TE")

    local longFlags=("number" "number-nonblank" "squeeze-blank")

    local testFiles=("test.txt" "ascii.txt")

    local testNumber=0

    for fileName in "${testFiles[@]}"; do

        cat "$fileName" > catOutput.txt
        ./s21_cat "$fileName" > s21CatOutput.txt
        if diff -q "catOutput.txt" "s21CatOutput.txt" > /dev/null; then
            echo "Test "$testNumber": success | flags : | file: "$fileName""
        else
            echo "Test "$testNumber": fail | flags : | file: "$fileName""
        fi
        ((testNumber++))

    done

    cat noFile.txt > catOutput.txt
    ./s21_cat noFile.txt > s21CatOutput.txt
    if diff -q "catOutput.txt" "s21CatOutput.txt" > /dev/null; then
        echo "Test "$testNumber": success | no file"
    else
        echo "Test "$testNumber": fail | no file"
    fi
    ((testNumber++))

    
    for fileName in "${testFiles[@]}"; do
        for value in "${flags[@]}"; do

            cat -"$value" "$fileName" > catOutput.txt
            ./s21_cat -"$value" "$fileName" > s21CatOutput.txt

            if diff -q "catOutput.txt" "s21CatOutput.txt" > /dev/null; then
                echo "Test "$testNumber": success | flags : -"$value" | file: "$fileName""
            else
                echo "Test "$testNumber": fail | flags : -"$value" | file: "$fileName""
            fi
            ((testNumber++))
        done
    done

    cat ascii.txt > catOutput.txt
    ./s21_cat ascii.txt > s21CatOutput.txt
    if diff -q "catOutput.txt" "s21CatOutput.txt" > /dev/null; then
        echo "Test "$testNumber": success | flags : | file: ascii.txt"
    else
        echo "Test "$testNumber": fail | flags : | file: ascii.txt"
    fi
    ((testNumber++))

    for fileName in "${testFiles[@]}"; do
        for value in "${longFlags[@]}"; do
            cat --"$value" "$fileName" > catOutput.txt
            ./s21_cat --"$value" "$fileName" > s21CatOutput.txt

            if diff -q "catOutput.txt" "s21CatOutput.txt" > /dev/null; then
                echo "Test "$testNumber": success | flags : -"$value" | file: "$fileName""
            else
                echo "Test "$testNumber": fail | flags : -"$value" | file: "$fileName""
            fi
            ((testNumber++))
        done
    done

}

main