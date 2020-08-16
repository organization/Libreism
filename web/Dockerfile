FROM node:14-alpine AS builder

RUN apk add --no-cache tzdata
ENV TZ Asia/Seoul

WORKDIR /usr/src/libreism

COPY package*.json ./
RUN npm ci

COPY . .
RUN npm run build
