//
// Sends 10 "hello" messages, 1 per second to specified destination mac address
//
// $src_ip
// $dst_ip
// $src_mac
// $dst_mac
// $dev
//

RatedSource(DATA "hello", RATE 1, LIMIT 10) -> IPEncap(200, $src_ip, $dst_ip)
						-> EtherEncap(0x0800, $src_mac, $dst_mac)
						-> Print(Sending)
						-> ToDevice($dev)
