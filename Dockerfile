FROM alpine:latest AS builder

RUN apk add --no-cache gcc musl-dev

COPY server.c server.c

RUN gcc -static -Os /server.c -o /app

FROM busybox:latest

LABEL org.opencontainers.image.authors="Szymon Kasperczuk"

COPY --from=builder /app /app

EXPOSE 8080

HEALTHCHECK --interval=120s --timeout=3s --retries=3 \
    CMD wget -qO- http://localhost:8080/ || exit 1

ENTRYPOINT ["/app"]