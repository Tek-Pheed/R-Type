FROM rust:latest

SHELL ["/bin/bash", "-c"]

WORKDIR /mdbook/

COPY . /mdbook/

RUN cargo install mdbook

EXPOSE 3000

CMD ["mdbook", "serve", "-n", "0.0.0.0"]