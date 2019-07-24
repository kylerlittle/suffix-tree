FROM srzzumix/googletest

# Establish workdir
ENV code_dir /usr/src/
WORKDIR ${code_dir}

# Copy current directory to code directory on Ubuntu image.
COPY . ${code_dir}

# Start bash and run program
CMD ["./run_tests.sh"]