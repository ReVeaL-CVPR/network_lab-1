
es :: EchoSender(MAXPACKETSIZE 50, RECEIVER er) 
	-> IPEncap(17, 192.168.1.1, DST_ANNO) 
	-> SetUDPChecksum()
	-> ToDump(test.dump, ENCAP IP)
	-> er :: EchoReceiver(SENDER es);