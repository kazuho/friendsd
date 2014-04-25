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

print S "localhost\n";

while($aLine = <S>) {
    print $aLine;
}

close(S);

