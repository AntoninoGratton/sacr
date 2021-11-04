FROM python:3.10
ENV PYTHONUNBUFFERED 1
RUN apt-get update

RUN mkdir /code
WORKDIR /code
ADD requirements.txt /code/
RUN pip3 install -r requirements.txt

ADD . /code/