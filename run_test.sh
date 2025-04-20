#!/bin/bash

EXEC="./cmake-build-debug/z4" # Path to the executable file
TEST_DIR="z4_testing" # Path to tests dir

for scenario in "$TEST_DIR"/stdin/scenar*/; do
    scenario_name=$(basename "$scenario")
    echo "$scenario_name:"

    for infile in "$scenario"example_*.txt; do
        testnum=$(basename "$infile" | sed -E 's/example_(.*)\.txt/\1/')
        expected_outfile="$TEST_DIR/stdout/$scenario_name/example_$testnum.txt"

        if [ ! -f "$expected_outfile" ]; then
            printf "   -- %-20s ❌ The expected output file is missing\n" "$testnum"
            continue
        fi

        if [[ "$scenario_name" == "scenar_2" ]]; then
            $EXEC 0 < "$infile" > temp_output.txt
        elif [[ "$scenario_name" == "scenar_4" ]]; then
            if [[ "$testnum" == "1" ]]; then
                $EXEC 10 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "2" ]]; then
                $EXEC 5 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "3" ]]; then
                $EXEC 14 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "4" ]]; then
                $EXEC 12 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "5" ]]; then
                $EXEC 11 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "6" ]]; then
                $EXEC 16 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "7" ]]; then
                $EXEC 10 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "8" ]]; then
                $EXEC 12 < "$infile" > temp_output.txt
            elif [[ "$testnum" == "9" ]]; then
                $EXEC 190 < "$infile" > temp_output.txt
            else
              printf " ERROR in scenar_4, witch param should be in example_$testnum ?"
            fi
        else
            $EXEC < "$infile" > temp_output.txt
        fi



        if diff temp_output.txt "$expected_outfile" --ignore-trailing-space >/dev/null; then
            printf "   -- %-20s ✅ Done\n" "$testnum"
        else
            printf "   -- %-20s ❌ False\n" "example_$testnum"
        fi
    done
done

rm -f temp_output.txt
