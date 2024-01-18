import asyncio
import functools
import struct
import sys


async def handle_client(payload, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
    writer.write(struct.pack('!i', len(payload)))
    writer.write(payload)
    await writer.drain()
    writer.close()


async def serve(endpoint, payload):
    host, port = endpoint.split(":")
    server = await asyncio.start_server(functools.partial(handle_client, payload), host, int(port))

    addrs = ', '.join(str(sock.getsockname()) for sock in server.sockets)
    print(f'Serving on {addrs}')

    async with server:
        await server.serve_forever()


def main():
    if len(sys.argv) != 3:
        print('Usage: server.py <listen> <file>')
        exit(1)
    with open(sys.argv[2], 'rb') as f:
        payload = f.read()
    asyncio.run(serve(sys.argv[1], payload))


if __name__ == '__main__':
    main()
