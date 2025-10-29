from random import randint
import socket
import sys
import re


HOST = '127.0.0.1'  # localhost
PORT = 9999         # Arbitrary non-privileged port


class DictXExtern():
    hash_table = {}


class DictXLocal():
    hash_table = {}


def insert(x: int, addr: str, port: str):
    if addr == HOST and port not in DictXLocal.hash_table.keys():
        DictXLocal.hash_table[port] = {"result": x}
        print(DictXLocal.hash_table[port])
        return

    DictXExtern.hash_table[addr] = {
        port: x
    }
    return


def main(fd):
    while True:
        data, tupla = fd.recvfrom(1024)
        data = data.decode("utf-8").strip()

        addr, port = tupla   # tupla == (ip, port)
        port = str(port)

        print(f"indirizzo: {addr}; porta: {port}; dati: ", data)

        if (addr == HOST) and (port not in DictXLocal.hash_table.keys()):
            insert(randint(1, 100), addr, port)

        elif addr not in DictXExtern.hash_table.keys():
            insert(randint(1, 100), addr, port)

        if not re.fullmatch(r"\d+", data):
            # ritorna a 'data, tupla = fd.recvfrom(4096)'
            fd.sendto("Errore! devi inserire solo numeri".encode('utf-8'), tupla)
            continue

        y = int(data)

        if addr != HOST:
            res = DictXExtern.hash_table[addr][port]
        else:
            print(DictXLocal.hash_table)
            res = DictXLocal.hash_table[port]["result"]

        if res == y:
            fd.sendto("Complimenti hai indovinato".encode('utf-8'), tupla)

            if addr != HOST:
                del DictXExtern.hash_table[addr][port]
            else:
                del DictXLocal.hash_table[port]

        elif res > y:
            fd.sendto("Error! numero troppo piccolo".encode('utf-8'), tupla)

        elif res < y:
            fd.sendto("Error! numero troppo grande".encode('utf-8'), tupla)


if __name__ == "__main__":
    # in os unix-like, un socket non e' altro che un file descriptor
    fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    fd.bind((HOST, PORT))

    print(f"Server listening on {HOST}:{PORT}")
    result = main(fd)
    print(f"uscito dal programma con valore {result}")
    sys.exit(result)
