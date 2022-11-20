# Pull base image.
FROM ubuntu:14.04

# Define working directory.
WORKDIR /root

COPY ./dist/yeet.ko /root

#RUN insmod /root/yeet.ko

# Define default command.
CMD ["bash"]