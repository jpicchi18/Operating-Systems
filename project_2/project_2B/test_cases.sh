# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu
    
    rm -f lab2b_list.csv

    # lab2b_1.png
    ./lab2_list --iterations=1000 --sync=m --threads=1 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=2 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=4 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=8 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=12 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=16 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=24 >> lab2b_list.csv

    ./lab2_list --iterations=1000 --sync=s --threads=1 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=2 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=4 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=8 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=12 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=16 >> lab2b_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=24 >> lab2b_list.csv

    # lab2b_2.png --> satisfied by first block of lab2b_1.png

    # lab2b_3.png
    ./lab2_list --yield=id --lists=4 --threads=1 --iterations=1 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=4 --iterations=1 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=8 --iterations=1 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=12 --iterations=1 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=16 --iterations=1 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=1 --iterations=2 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=4 --iterations=2 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=8 --iterations=2 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=12 --iterations=2 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=16 --iterations=2 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=1 --iterations=4 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=4 --iterations=4 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=8 --iterations=4 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=12 --iterations=4 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=16 --iterations=4 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=1 --iterations=8 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=4 --iterations=8 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=8 --iterations=8 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=12 --iterations=8 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=16 --iterations=8 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=1 --iterations=16 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=4 --iterations=16 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=8 --iterations=16 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=12 --iterations=16 >> lab2b_list.csv
    ./lab2_list --yield=id --lists=4 --threads=16 --iterations=16 >> lab2b_list.csv

    ./lab2_list --yield=id --sync=s --lists=4 --threads=1 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=4 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=8 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=12 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=16 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=1 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=4 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=8 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=12 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=16 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=1 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=4 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=8 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=12 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=16 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=1 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=4 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=8 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=12 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=s --lists=4 --threads=16 --iterations=80 >> lab2b_list.csv

    ./lab2_list --yield=id --sync=m --lists=4 --threads=1 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=4 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=8 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=12 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=16 --iterations=10 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=1 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=4 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=8 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=12 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=16 --iterations=20 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=1 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=4 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=8 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=12 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=16 --iterations=40 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=1 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=4 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=8 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=12 --iterations=80 >> lab2b_list.csv
    ./lab2_list --yield=id --sync=m --lists=4 --threads=16 --iterations=80 >> lab2b_list.csv

    # lab2b_4.png and lab2b_5.png
    ./lab2_list --sync=m --iterations=1000 --threads=1 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=2 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=4 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=8 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=12 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=1 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=2 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=4 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=8 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=12 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=1 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=2 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=4 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=8 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=m --iterations=1000 --threads=12 --lists=16 >>lab2b_list.csv

    ./lab2_list --sync=s --iterations=1000 --threads=1 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=2 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=4 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=8 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=12 --lists=4 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=1 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=2 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=4 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=8 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=12 --lists=8 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=1 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=2 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=4 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=8 --lists=16 >>lab2b_list.csv
    ./lab2_list --sync=s --iterations=1000 --threads=12 --lists=16 >>lab2b_list.csv
