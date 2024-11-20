import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
csv_file = "performance_results.csv"  # Update the file name if different
df = pd.read_csv(csv_file)

# Compute the mean and standard deviation of execution time for each thread configuration
grouped = df.groupby("Threads")["Time (s)"]
mean_times = grouped.mean()
std_times = grouped.std()

# Plot the results
plt.figure(figsize=(10, 6))
plt.errorbar(
    mean_times.index, 
    mean_times.values, 
    yerr=std_times.values, 
    fmt='o-',  # Circle marker and solid line
    ecolor='red',  # Error bar color
    capsize=5,  # Error bar cap size
    label="Execution Time with Error Bars"
)

# Customize the plot
plt.title("Execution Time vs. Total Threads", fontsize=14)
plt.xlabel("Total Threads", fontsize=12)
plt.ylabel("Execution Time (s)", fontsize=12)
plt.xticks(mean_times.index, fontsize=10)
plt.yticks(fontsize=10)
plt.ylim(0)  # Ensure the Y-axis starts at 0
plt.grid(True)
plt.legend(fontsize=12)

# Save the plot as an image file (optional)
#plt.savefig("performance_with_error_plot.png")

# Show the plot
plt.show()