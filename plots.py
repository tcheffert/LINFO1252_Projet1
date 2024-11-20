import pandas as pd
import matplotlib.pyplot as plt

# File paths
pc_csv = "performance_producers_consumers.csv"
rw_csv = "performance_readers_writers.csv"

# Load data
pc_data = pd.read_csv(pc_csv)
rw_data = pd.read_csv(rw_csv)

def plot_performance(data, title, output_file):
    """
    Plots the execution time vs. number of threads with mean and std deviation.
    
    Args:
        data (pd.DataFrame): The input data with columns 'Threads', 'Run', and 'Time (s)'.
        title (str): Title of the graph.
        output_file (str): File path to save the plot.
    """
    # Group by the number of threads and calculate mean and std deviation
    grouped = data.groupby("Threads")["Time (s)"]
    means = grouped.mean()
    stds = grouped.std()

    # Plot the results
    plt.figure(figsize=(10, 6))
    plt.errorbar(means.index, means, yerr=stds, fmt='-o', capsize=5, label="Execution Time")
    
    # Graph settings
    plt.title(title)
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (s)")
    plt.ylim(0)  # y-axis starts at 0
    plt.xticks(means.index)  # Ensure x-axis matches thread counts
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    
    # Show the plot
    #plt.savefig(output_file)
    plt.show()

# Plot for Producers/Consumers
plot_performance(pc_data, "Performance: Producers/Consumers Problem", "producers_consumers_performance.png")

# Plot for Readers/Writers
plot_performance(rw_data, "Performance: Readers/Writers Problem", "readers_writers_performance.png")
