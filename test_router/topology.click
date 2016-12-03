require(library /home/comnetsii/elements/lossyrouterport.click);


client :: BasicClient(MY_ADDRESS $sip, OTHER_ADDRESS $tip, TIME_OUT 3);
rp1 :: LossyRouterPort(DEV $dev1, IN_MAC $in_mac1 , OUT_MAC $out_mac1, LOSS 1.0, DELAY 0.2 );
rp2 :: LossyRouterPort(DEV $dev2, IN_MAC $in_mac2 , OUT_MAC $out_mac2, LOSS 1.0, DELAY 0.2 );
router::GraphBuilder(MY_ADDRESS $rip, DELAY 1, TIME_OUT 5);


router[0] -> rp1 -> [0]router
router[1] -> rp2 -> [1]router
client -> [2]router[2] -> client
