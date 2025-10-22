import socket
import sys


HOST = '127.0.0.1'  # Server's address (localhost in this case)
PORT = 9999        # Server's port


def main(fd) -> int:
    # fd.sendto("".encode('utf-8'), (HOST, PORT))
    msg_from_server = ""
    while msg_from_server != "Complimenti hai indovinato":
        fd.sendto(
            input("Inserisci numero: ").encode('utf-8'),
            (HOST, PORT)
        )
        msg_from_server, tupla = fd.recvfrom(1024)
        msg_from_server = msg_from_server.decode("utf-8")
        print(msg_from_server)

    return 0


if __name__ == "__main__":
    fd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    print(f"Connected to server at {HOST}:{PORT}")

    result = main(fd)

    print(f"uscito dal programma con valore {result}")
    sys.exit(result)
