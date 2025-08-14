main() {

    local flags=("e" "i" "v" "c" "l" "n" "h" "s"
    "ie" "iv" "ic" "il" "in" "ih" "is" 
    "ve" "vi" "vc" "vl" "vn" "vh" "vs" 
    "ce" "ci" "cv" "cl" "cn" "ch" "cs" 
    "le" "li" "lv" "lc" "ln" "lh" "ls" 
    "ne" "ni" "nv" "nc" "nl" "nh" "ns" 
    "he" "hi" "hv" "hc" "hl" "hn" "hs" 
    "se" "si" "sv" "sc" "sl" "sn" "sh")

    local patterns=("hello" "[avg]" "[1-9]" "Gg")

    local fileNames=("test.txt" "test2.txt")

    local testNumber=0

    grep hello test.txt > grepOutput.txt
    ./s21_grep hello test.txt > s21GrepOutput.txt
    if diff -q "grepOutput.txt" "s21GrepOutput.txt" > /dev/null; then
        echo "Test "$testNumber": success | flags : | file : | test.txt | pattern : hello"
    else
        echo "Test "$testNumber": fail | flags : | test.txt | pattern : hello"
    fi
    ((testNumber++))

    for fileName in "${fileNames[@]}"; do
        for pattern in "${patterns[@]}"; do
            for value in "${flags[@]}"; do

                grep -"$value" "$pattern" "$fileName" > grepOutput.txt
                ./s21_grep -"$value" "$pattern" "$fileName" > s21GrepOutput.txt

                if diff -q "grepOutput.txt" "s21GrepOutput.txt" > /dev/null; then
                    echo "Test "$testNumber": success | flags : -"$value" | file: "$fileName" | pattern: "$pattern""
                else
                    echo "Test "$testNumber": fail | flags : -"$value" | file: "$fileName" | pattern: "$pattern""
                fi
                ((testNumber++))
            done
        done
    done

    for pattern in "${patterns[@]}"; do
        for value in "${flags[@]}"; do

            grep -"$value" "$pattern" test.txt test2.txt > grepOutput.txt
            ./s21_grep -"$value" "$pattern" test.txt test2.txt > s21GrepOutput.txt

            if diff -q "grepOutput.txt" "s21GrepOutput.txt" > /dev/null; then
                echo "Test "$testNumber": success | flags : -"$value" | files: test.txt test2.txt | pattern: "$pattern""
            else
                echo "Test "$testNumber": fail | flags : -"$value" | files: test.txt test2.txt | pattern: "$pattern""
            fi
            ((testNumber++))
        done
    done

    grep hello nofile.txt > grepOutput.txt
    ./s21_grep hello nofile.txt > s21GrepOutput.txt
    if diff -q "grepOutput.txt" "s21GrepOutput.txt" > /dev/null; then
        echo "Test "$testNumber": success | flags : | file : | nofile.txt | pattern : hello"
    else
        echo "Test "$testNumber": fail | flags : | nofile.txt | pattern : hello"
    fi
    ((testNumber++))

}

main