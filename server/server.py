from random import randint
import socket
import sys
import re


HOST = '127.0.0.1'  # localhost
PORT = 9999         # Arbitrary non-privileged port


def main(fd):
    while True:  # ascoltiamo e serviamo in sequenza clienti infiniti
        x = randint(1, 100)
        conn, addr = fd.accept()
        print(f"Mi si e' connesso {addr}")
        conn.sendall(b'Buongiorno, indovina il numero')

        while True:
            # la comunicazione col cliente e' potenzialmente infinita
            data = conn.recv(1024).decode().strip()

            if not data:
                break

            print(data)

            if not re.fullmatch(r"\d+", data):
                # devo verificare prima che sia un numero davvero, altrimenti
                # se passo a int() dei char che non sono numeri crasha
                # quindi uso una regex per verificare che non ha messo altro
                conn.sendall(b"Devi inserire un numero da 1 a 100!")
                continue

            n_client = int(data)

            if n_client == x:
                print(f"connessione chiusa per {addr}")
                conn.sendall(b"Hai indovinato!")
                conn.close()
                break

            if n_client > x:
                conn.sendall(b"No, piu' basso!")
                continue

            if n_client < x:
                conn.sendall(b"No, piu' alto!")


if __name__ == "__main__":
    # in os unix-like, un socket non e' altro che un file descriptor
    fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fd.bind((HOST, PORT))
    fd.listen()
    print(f"Server listening on {HOST}:{PORT}")
    result = main(fd)
    print(f"uscito dal programma con valore {result}")
    sys.exit(result)
