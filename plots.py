import pandas as pd
import matplotlib.pyplot as plt

# File paths
pc_csv = "performance_data/performance_producers_consumers.csv"
pc_sem_csv = "performance_data/performance_prod_conso_sem.csv"
rw_csv = "performance_data/performance_readers_writers.csv"
rw_sem_csv = "performance_data/performance_readers_writers_sem.csv"
philo_csv = "performance_data/performance_philosophers.csv"
philo_sem_csv = "performance_data/performance_philo_sem.csv"


# Load data
pc_data = pd.read_csv(pc_csv)
pc_sem_data = pd.read_csv(pc_sem_csv)
rw_data = pd.read_csv(rw_csv)
rw_sem_data = pd.read_csv(rw_sem_csv)
philo_data = pd.read_csv(philo_csv)
philo_sem_data = pd.read_csv(philo_sem_csv)


def plot_performance(data, title, output_file, color="blue", pdf=False):
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
    plt.errorbar(means.index, means, yerr=stds, fmt='-o', color=color, capsize=7, label="Execution time with standard deviation")
    
    # Graph settings
    plt.title(title)
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time [s]")
    plt.ylim(0)  # y-axis starts at 0
    plt.xticks(means.index)  # Ensure x-axis matches thread counts
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    
    # Show the plot
    if pdf == True:
        plt.savefig('plots/' + output_file + ".pdf", format='pdf', bbox_inches="tight")
    
    plt.savefig('plots/' + output_file + ".png")
    plt.show()

intoPdf = True

# Plot for Producers/Consumers with new semaphore interface
#plot_performance(pc_data, "Performance Producers/Consumers depending on the numbers of Threads", "producers_consumers_performance", color="cornflowerblue", pdf=intoPdf)
# Plot for Producers/Consumers with new semaphore interface
#plot_performance(pc_sem_data, "Performance Producers/Consumers with our semaphore and lock interface depending on the numbers of Threads", "prod_conso_sem_performance", color="slateblue", pdf=intoPdf)


# Plot for Readers/Writers
#plot_performance(rw_data, "Performance Readers/Writers depending on the numbers of Threads", "readers_writers_performance", color="mediumseagreen", pdf=intoPdf)
# Plot for Readers/Writers with new semaphore interface
#plot_performance(rw_sem_data, "Performance Readers/Writers with our semaphore and lock interface depending on the numbers of Threads", "readers_writers_sem_performance", color="mediumaquamarine", pdf=intoPdf)


# Plot for Philosophers
plot_performance(philo_data, "Performance Philosophers depending on the numbers of Threads", "philosophers_performance", color="crimson", pdf=intoPdf)
# Plot for Philosophers
#plot_performance(philo_sem_data, "Performance Philosophers with our semaphore and lock interface depending on the numbers of Threads", "philo_sem_performance", color="red", pdf=intoPdf)
