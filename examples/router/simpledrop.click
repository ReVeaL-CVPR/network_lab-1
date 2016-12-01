RatedSource(DATA "hello-longer-than-ten", RATE 1, LIMIT 10) -> SimplePushElement(MAXPACKETSIZE 10) -> Print("It passed our element") -> Discard();
 
