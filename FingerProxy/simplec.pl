#!/usr/local/bin/perl

($aHost, $aPort) = @ARGV;
$aPort = 7220 unless $aPort;
$aHost = 'ecc-as50.komaba.ecc.u-tokyo.ac.jp' unless $aHost;

$AF_INET = 2;
$SOCK_STREAM = 1;

$aSockaddr = 'S n a4 x8';
chop($aLocalhost = `hostname`);

($aName, $aAliases, $aProto) = getprotobyname('tcp');
# ($aName, $aAliases, $aPort) = getservbyname($aPort, 'tcp')
#     unless $aPort =~ /^\d+$/;
# ($aName, $aAliases, $aType, $aLen, $aThisAddr) =
#     gethostbyname($aLocalhost);
($aName, $aAliases, $aType, $aLen, $aThatAddr) =
    gethostbyname($aHost);

$aThis = pack($aSockaddr, $AF_INET, 0, $aThisAddr);
$aThat = pack($aSockaddr, $AF_INET, $aPort, $aThatAddr);

socket(S, $AF_INET, $SOCK_STREAM, $aProto) || die $!;
# bind(S, $aThis) || die $!;
connect(S, $aThat) || die $!;

select(S); $| = 1;
select(STDERR); $| = 1;
select(STDOUT); $| = 1;

print S "ecc-as50 ecc-as51 ecc-as52 ecc-as53 xss01 xss02 xss03 xss04 xss05 xss06 xss07 xss08 xss09 xss10 xss11 xss12 xss13 xss14 xss15 xss16 xss17 xss18 xss19 xss20 xss21 xss22 xss23 xss24 xss25 xss26 xss27 xss28 xss29 xss30 xss31 xss32 xss33 xss34 xss35 xss36 xss37 xss38 xss39 xss40 xss41 xss42 xss43 xss44 xss45 xss46 xss47 xss48 xss49 xss50 xss51 xss52 xss53 xst01 xst02 xst03 xst04 xst05 xst06 xst07 xst08 xst09 xst10 xst11 xst12 xst13 xst14 xst15 xst16 xst17 xst18 xst19 xst20 xst21 xst22 xst23 xst24 xst25 xst26 xst27 xst28 ecc-xs51 ecc-xs52 ecc-xs53 ecc-xs54 ecc-xs55 ecc-xs56 ecc-xs57 ecc-xs58 ecc-xs59 ecc-xs60 ecc-xs61 ecc-xs62 ecc-xs63 ecc-xs64 ecc-xs65 ecc-xs66 ecc-xs67 ecc-xs68 ecc-xs69 ecc-xs70 ecc-xs71 ecc-xs72 ecc-xs73 ecc-xs74 ecc-xs75 ecc-xs76 ecc-xs77 ecc-xs78 ecc-xs79 ecc-xs80 ecc-xs81 ecc-xs82 ecc-xs83 ecc-xs84 ecc-xs85 c00-xs00 xsi01 n00-xs00\n";

while($aLine = <S>) {
    print $aLine;
}

close(S);

