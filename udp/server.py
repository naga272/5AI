from random import randint
import socket
import sys
import re


HOST = '127.0.0.1'  # localhost
PORT = 9999         # Arbitrary non-privileged port


hash_table = {}


def insert(x: int, addr: str, port: str):
    hash_table[addr] = {
        port: x
    }
    print(f"ho fatto la insert: {hash_table}")


def main(fd):
    while True:
        data, tupla = fd.recvfrom(1024)
        data = data.decode("utf-8").strip()

        addr, port = tupla   # tupla == (ip, port)
        print(f"indirizzo: {addr}; porta: {port}; dati: ", data)

        if addr not in hash_table.keys():
            insert(
                randint(1, 100),
                str(addr),
                str(port)
            )

        if str(port) not in hash_table[addr].keys():
            insert(
                randint(1, 100),
                str(addr),
                str(port)
            )

        if not re.fullmatch(r"\d+", data):
            fd.sendto("Errore! devi inserire solo numeri".encode('utf-8'), tupla)
            continue  # ritorna a 'data, tupla = fd.recvfrom(4096)'

        y = int(data)

        if hash_table[addr][str(port)] == y:
            fd.sendto("Complimenti hai indovinato".encode('utf-8'), tupla)

        if hash_table[addr][str(port)] > y:
            fd.sendto("Error! numero troppo piccolo".encode('utf-8'), tupla)

        if hash_table[addr][str(port)] < y:
            fd.sendto("Error! numero troppo grande".encode('utf-8'), tupla)


if __name__ == "__main__":
    # in os unix-like, un socket non e' altro che un file descriptor
    fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    fd.bind((HOST, PORT))

    print(f"Server listening on {HOST}:{PORT}")
    result = main(fd)
    print(f"uscito dal programma con valore {result}")
    sys.exit(result)
