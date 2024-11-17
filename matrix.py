import random

def generate_random_matrix(size, filename):
    with open(filename, "w") as file:
        for i in range(size):
            row = []
            for j in range(size):
                if i == j:  # Keep the diagonal as 0
                    row.append(0)
                else:  # Replace 1 with a random number between 2 and 100
                    row.append(random.randint(2, 25))
            file.write(" ".join(map(str, row)) + "\n")

# Specify the size of the matrix
matrix_size = 26
output_file = "random_matrix.txt"

generate_random_matrix(matrix_size, output_file)
print(f"Matrix saved to {output_file}")
