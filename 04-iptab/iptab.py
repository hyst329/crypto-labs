import argparse
import ipaddress
import subprocess


def check_port(s):
    try:
        x = int(s)
        return (x, x)
    except:
        try:
            x, y = [int(t) for t in s.split("-")]
            if x > y:
                raise
            return (x, y)
        except:
            raise argparse.ArgumentTypeError(
                "%s is not a valid port range" % s)


def check_ip(s):
    try:
        x = ipaddress.ip_address(s)
        return (x, x)
    except:
        try:
            x, y = [ipaddress.ip_address(t) for t in s.split("-")]
            if x > y:
                raise
            return (x, y)
        except:
            raise argparse.ArgumentTypeError("%s is not a valid IP range" % s)


def get_ip_range(ips):
    if type(ips[0]) is not tuple:
        ips = [ips]
    nws = [list(ipaddress.summarize_address_range(
        ip_range[0], ip_range[1])) for ip_range in ips]
    nws = [item for sublist in nws for item in sublist]
    ips = {ip for nw in nws for ip in nw}
    return sorted(ips)


def get_port_range(ports):
    if type(ports[0]) is not tuple:
        ports = [ports]
    ranges = [range(x, y + 1) for (x, y) in ports]
    ports = {port for r in ranges for port in r}
    return sorted(ports)


parser = argparse.ArgumentParser(description='Invoke iptables.')
parser.add_argument("--ips", nargs="+", default="192.168.1.1-192.168.1.100",
                    help="A single IP address or a range of IP addresses"
                    " (e.g. 127.0.0.1 or 192.168.1.1-192.168.1.254)",
                    type=check_ip)
parser.add_argument("--ports", nargs="+", default="1-1000",
                    help="A single port or a range of ports"
                    " (e.g. 80 or 443-454)",
                    type=check_port)
parser.add_argument("--timeout", default=100,
                    help="Timeout value in milliseconds", type=int)
args = parser.parse_args()
ips = get_ip_range(args.ips)
ports = get_port_range(args.ports)

for ip in ips:
    print("*** Scanning ip %s ***" % ip)
    nmap_ports = ",".join(str(p) for p in ports)
    res = subprocess.check_output("nmap -oG - --reason -p %s %s"
                                  % (nmap_ports, ip))
    i = res.find(b"Ignored State")
    istate = res[i + 15:res.find(b" ", i + 16)].decode("utf-8") if i >= 0 else "unlisted"
    ps = res.find(b"Ports:")
    for port in ports:
        print("Checking port %s..." % port, end='')
        b = res.find(b"%d/" % port, ps + 1)
        if b >= 0: b += len(b"%d/" % port)
        e = res.find(b"/", b + 1)
        print(res[b:e].decode("utf-8") if b >= 0 else istate)
