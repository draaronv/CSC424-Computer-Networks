

IPADDR = 18.191.176.89
E = evidence.out

test:
	wget ${IPADDR} -O $E
	echo "---------------" >> $E
	echo "uname:" `uname -a` >> $E
	echo "hostname:" `hostname` >> $E
	echo "IP Address:" ${IPADDR} >>  $E
	echo `date` >> $E
	echo "now svn add and commit $E"

	
