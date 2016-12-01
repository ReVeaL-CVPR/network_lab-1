require(library /home/comnetsii/elements/routerport.click);

rp1 :: RouterPort(DEV veth2, IN_ADDR 192.168.1.2, OUT_ADDR 192.168.1.1, IN_PORT 10001, OUT_PORT 10000, IN_MAC 5a:54:16:dd:41:4d, OUT_MAC d2:82:a7:64:c6:cd);
rp2 :: RouterPort(DEV veth3, IN_ADDR 192.168.4.2, OUT_ADDR 192.168.4.1, IN_PORT 10002, OUT_PORT 10003, IN_MAC 5e:a3:55:25:28:c9, OUT_MAC 32:61:51:3c:e4:75);

rp1 -> Print(Received, MAXLENGTH -1) -> rp2;
rp2 -> Print(Received, MAXLENGTH -1) -> rp1;

