import pandas as pd
import matplotlib.pyplot as plt

#Helper function to get the data's file path
def csvData(directory:str, name:str):
    formatFile = ".csv"
    return directory + name + formatFile

#=== File paths ===#
repo = "performance/"
#Prod/cons
pc_csv = csvData(repo, "pc_POSIX")
pc_TAS_csv = csvData(repo, "pc_TAS")
pc_TATAS_csv = csvData(repo, "pc_TATAS")
#Readers/Writers problem
rw_csv = csvData(repo, "rw_POSIX")
rw_TAS_csv = csvData(repo, "rw_TAS")
rw_TATAS_csv = csvData(repo, "rw_TATAS")
#Philosophers problem
philo_csv = csvData(repo, "philo_POSIX")
philo_TAS_csv = csvData(repo, "philo_TAS")
philo_TATAS_csv = csvData(repo, "philo_TATAS")
#Test and Set
test_and_set_csv = csvData(repo, "test_and_set")
#Test and Test and Set
test_and_test_and_set_csv_studsrv = csvData(repo, "test_and_test_and_set")
#test_and_test_and_set_csv_30Runs = csvData(repo, "test_and_test_and_set30Runs")



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
#Test and Set
test_and_set_data = pd.read_csv(test_and_set_csv)
#Test and Test and Set
test_and_test_and_set_data_studsrv = pd.read_csv(test_and_test_and_set_csv_studsrv)
#test_and_test_and_set_data_30Runs = pd.read_csv(test_and_test_and_set_csv_30Runs)



def plot_performance(data, yMax, title, output_file, color="blue", saveFig=False):
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
    plt.title(title, weight='bold')
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time [s]")
    plt.ylim(0, yMax)  # y-axis starts at 0 and go to yMax
    plt.xticks(means.index)  # Ensure x-axis matches thread counts
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()
    
    # Show the plot
    if saveFig == True:
        plt.savefig('plots/' + output_file + ".pdf", format='pdf', bbox_inches="tight")
    plt.show()

save = True

#===Plot for Producers/Consumers===#
plot_performance(pc_data, 2.25, "Performance Producers/Consumers POSIX depending on the numbers of Threads", "pc_POSIX_scaled", color="cornflowerblue", saveFig=save)
plot_performance(pc_data_TAS, 2.25, "Performance Producers/Consumers TAS depending on the numbers of Threads", "pc_TAS_scaled", color="slateblue", saveFig=save)
plot_performance(pc_data_TATAS, 2.25, "Performance Producers/Consumers TATAS depending on the numbers of Threads", "pc_TATAS_scaled", color="cornflowerblue", saveFig=save)

#===Plot for Readers/Writers===#
plot_performance(rw_data, 0.6,  "Performance Readers/Writers POSIX depending on the numbers of Threads", "rw_POSIX_scaled", color="mediumseagreen", saveFig=save)
plot_performance(rw_data_TAS, 0.6, "Performance Readers/Writers TAS depending on the numbers of Threads", "rw_TAS_scaled", color="mediumaquamarine", saveFig=save)
plot_performance(rw_data_TATAS, 0.6, "Performance Readers/Writers TATAS depending on the numbers of Threads", "rw_TATAS_scaled", color="forestgreen", saveFig=save)

#===Plot for Philosophers===#
plot_performance(philo_data, 8, "Performance Philosophers POSIX depending on the numbers of Threads", "philo_POSIX_scaled", color="crimson", saveFig=save)
plot_performance(philo_data_TAS, 8, "Performance Philosophers TAS depending on the numbers of Threads", "philo_TAS_scaled", color="red", saveFig=save)
plot_performance(philo_data_TATAS, 8, "Performance Philosophers TATAS depending on the numbers of Threads", "philo_TATAS_scaled", color="darkred", saveFig=save)

#===Plot for Test and Set===#
plot_performance(test_and_set_data, 0.8, "Performance Test and Set depending on the numbers of Threads", "test_and_set_scaled", color="darkorchid", saveFig=save)

#===Plot for Test and Test and Set===#
plot_performance(test_and_test_and_set_data_studsrv, 0.8, "Performance Test and Test and Set depending on the numbers of Threads", "test_and_test_and_set_scaled", color="darkorange", saveFig=save)
#plot_performance(test_and_test_and_set_data_30Runs, "Performance Test and Test and Set with 30 runs depending on the numbers of Threads", "test_and_test_and_set30Runs", color="darkgoldenrod", saveFig=save)