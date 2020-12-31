# NAME: Joseph Picchi
# EMAIL: jpicchi22@g.ucla.edu
    
    rm -f lab2_add.csv lab2_list.csv
	# lab2_add-1.png
	./lab2_add --yield --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=100000 >> lab2_add.csv
	# lab2_add-2.png
    ./lab2_add --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
    ./lab2_add --threads=4 --iterations=10 >> lab2_add.csv
    ./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
    ./lab2_add --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
    ./lab2_add --threads=12 --iterations=10 >> lab2_add.csv
    ./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv
	# lab2_add-3.png --> good
	./lab2_add --iterations=10 >> lab2_add.csv
	./lab2_add --iterations=100 >> lab2_add.csv
	./lab2_add --iterations=1000 >> lab2_add.csv
	./lab2_add --iterations=10000 >> lab2_add.csv
	./lab2_add --iterations=100000 >> lab2_add.csv
	# lab2_add-4.png --> good
	./lab2_add --yield --sync=m --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=12 --iterations=10000 >> lab2_add.csv

	./lab2_add --yield --sync=c --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=12 --iterations=10000 >> lab2_add.csv

	./lab2_add --yield --sync=s --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=12 --iterations=1000 >> lab2_add.csv
	# lab2_add-5.png --> good
    ./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
    ./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv

	./lab2_add --sync=m --iterations=10000 --threads=1 >> lab2_add.csv
	./lab2_add --sync=m --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --sync=m --iterations=10000 --threads=4 >> lab2_add.csv
	./lab2_add --sync=m --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --sync=m --iterations=10000 --threads=12 >> lab2_add.csv

	./lab2_add --sync=s --iterations=10000 --threads=1 >> lab2_add.csv
	./lab2_add --sync=s --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --sync=s --iterations=10000 --threads=4 >> lab2_add.csv
	./lab2_add --sync=s --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --sync=s --iterations=10000 --threads=12 >> lab2_add.csv

	./lab2_add --sync=c --iterations=10000 --threads=1 >> lab2_add.csv
	./lab2_add --sync=c --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --sync=c --iterations=10000 --threads=4 >> lab2_add.csv
	./lab2_add --sync=c --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --sync=c --iterations=10000 --threads=12 >> lab2_add.csv


    #lab2_list-1.png
    ./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
    ./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
    ./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
    ./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
    ./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv

    #lab2_list-2.png
    ./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

    ./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

    ./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

    ./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

    ./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv

    #lab2_list-3.png
    ./lab2_list --threads=12 --iterations=32 --sync=m --yield=i >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=m --yield=d >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=m --yield=il >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=m --yield=dl >>lab2_list.csv

    ./lab2_list --threads=12 --iterations=32 --sync=s --yield=i >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=s --yield=d >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=s --yield=il >>lab2_list.csv
    ./lab2_list --threads=12 --iterations=32 --sync=s --yield=dl >>lab2_list.csv

    #lab2_list-4.png
    #lab2_list-5.png
    ./lab2_list --iterations=1000 --threads=1 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=1 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=2 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=4 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=8 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=12 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=16 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=m --threads=24 >>lab2_list.csv

    ./lab2_list --iterations=1000 --sync=s --threads=1 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=2 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=4 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=8 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=12 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=16 >>lab2_list.csv
    ./lab2_list --iterations=1000 --sync=s --threads=24 >>lab2_list.csv