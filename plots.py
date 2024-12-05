import pandas as pd
import matplotlib.pyplot as plt

#Helper function to get the data's file path
def csvData(name:str):
    repo = "performance/"
    formatFile = ".csv"
    return repo + name + formatFile
#=== File paths ===#
#Prod/cons
pc_csv = "performance_data/performance_producers_consumers.csv"
pc_TAS_csv = "performance_data/perf_pc_TAS.csv"
pc_TATAS_csv = "performance_data/perf_pc_TATAS.csv"
#Readers/Writers problem
rw_csv = csvData("rw_POSIX")
rw_TAS_csv = csvData("rw_TAS")
rw_TATAS_csv = csvData("rw_TATAS")
#Philosophers problem
philo_csv = "performance_data/performance_philosophers.csv"
philo_TAS_csv = "performance_data/perf_philo_TAS.csv"
philo_TATAS_csv = "performance_data/perf_philo_TATAS.csv"



#=== Load data ===#
#Prod/cons
pc_data = pd.read_csv(pc_csv)
pc_data_TAS = pd.read_csv(pc_TAS_csv)
pc_data_TATAS = pd.read_csv(pc_TATAS_csv)
#Readers/Writers
rw_data = pd.read_csv(rw_csv)
rw_data_TAS = pd.read_csv(rw_TAS_csv)
rw_data_TATAS = pd.read_csv(rw_TATAS_csv)
#Philosophers
philo_data = pd.read_csv(philo_csv)
philo_data_TAS = pd.read_csv(philo_TAS_csv)
philo_data_TATAS = pd.read_csv(philo_TATAS_csv)



def plot_performance(data, title, output_file, color="blue", saveFig=False, pdf=False):
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
    if saveFig == True:
        if pdf == True:
            plt.savefig('plots/' + output_file + ".pdf", format='pdf', bbox_inches="tight")
        
        plt.savefig('plots/' + output_file + ".png")
    plt.show()

intoPdf = True

# Plot for Producers/Consumers with new semaphore interface
#plot_performance(pc_data, "Performance Producers/Consumers depending on the numbers of Threads", "producers_consumers_performance", color="cornflowerblue", pdf=intoPdf)
#plot_performance(pc_data_TAS, "Performance Producers/Consumers with our semaphore and lock interface depending on the numbers of Threads", "prod_conso_perf_TAS", color="slateblue", pdf=intoPdf)
#plot_performance(pc_data_TATAS, "Performance Producers/Consumers depending on the numbers of Threads", "prod_cons_perf_TATAS", color="cornflowerblue", pdf=intoPdf)

#===Plot for Readers/Writers===#
plot_performance(rw_data, "Performance Readers/Writers POSIX depending on the numbers of Threads", "readers_writers_performance", color="mediumseagreen")
plot_performance(rw_data_TAS, "Performance Readers/Writers TAS depending on the numbers of Threads", "readers_writers_TAS_performance", color="mediumaquamarine")
plot_performance(rw_data_TATAS, "Performance Readers/Writers TATAS depending on the numbers of Threads", "readers_writers__TATAS_performance", color="mediumseagreen")


#===Plot for Philosophers===#
#plot_performance(philo_data, "Performance Philosophers POSIX depending on the numbers of Threads", "philosophers_performance", color="crimson", pdf=intoPdf)
#plot_performance(philo_data_TAS, "Performance Philosophers TAS depending on the numbers of Threads", "philo_TAS_performance", color="red", pdf=intoPdf)
#plot_performance(philo_data_TATAS, "Performance Philosophers TATAS depending on the numbers of Threads", "philo_TATAS_performance", color="darkred", pdf=intoPdf)
