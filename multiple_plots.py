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
test_and_set_csv_studsrv = csvData(repo, "test_and_set")
test_and_set_csv_local = csvData("performance_local/", "test_and_set")
#Test and Test and Set
test_and_test_and_set_csv_studsrv = csvData(repo, "test_and_test_and_set")
test_and_test_and_set_csv_30Runs = csvData(repo, "test_and_test_and_set30Runs")




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
test_and_set_data_studsrv = pd.read_csv(test_and_set_csv_studsrv)
test_and_set_data_local = pd.read_csv(test_and_set_csv_local)
#Test and Test and Set
test_and_test_and_set_data_studsrv = pd.read_csv(test_and_test_and_set_csv_studsrv)
test_and_test_and_set_data_30Runs = pd.read_csv(test_and_test_and_set_csv_30Runs)



# Function to plot multiple datasets
def plot_multiple_datasets(datasets, labels, title, output_file, colors=None, saveFig=False, pdf=False):
    """
    Plots multiple datasets on the same plot.

    Args:
        datasets (list of pd.DataFrame): List of dataframes to plot.
        labels (list of str): Corresponding labels for the datasets.
        title (str): Title of the graph.
        output_file (str): File path to save the plot.
        colors (list of str): List of colors for the plots.
    """
    plt.figure(figsize=(10, 6))
    
    # Default colors if not provided
    if not colors:
        colors = ['blue', 'green', 'red']
    
    # Plot each dataset
    for i, data in enumerate(datasets):
        grouped = data.groupby("Threads")["Time (s)"]
        means = grouped.mean()
        stds = grouped.std()
        plt.errorbar(
            means.index,
            means,
            yerr=stds,
            fmt='-o',
            color=colors[i % len(colors)],
            capsize=5,
            label=labels[i]
        )
    
    # Graph settings
    plt.title(title)
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time [s]")
    plt.ylim(0)  # y-axis starts at 0
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.legend()

    # Set x-axis ticks explicitly for clarity
    thread_counts = [2, 4, 8, 16, 32]
    plt.xticks(thread_counts, labels=thread_counts)

    # Save the plot if needed
    if saveFig:
        if pdf:
            plt.savefig('plots/' + output_file + ".pdf", format='pdf', bbox_inches="tight")
        plt.savefig('plots/' + output_file + ".png")
    plt.show()

# Plot for Producers/Consumers
"""
plot_multiple_datasets(
    datasets=[test_and_set_data_local, test_and_set_data_studsrv],
    labels=["Local", "Studsrv"],
    title="Performance Test and Set",
    output_file="test_and_set_comparaison",
    colors=["blue", "green"],
    saveFig=False,
    pdf=False
)
"""
# Plot for task 2.3
plot_multiple_datasets(
    datasets=[test_and_set_data_studsrv, test_and_test_and_set_data_studsrv],
    labels=["Test and Set", "Test and Test and Set"],
    title="Performance Test and Set vs Test and Test and Set",
    output_file="TAS_vs_TATAS_comparaison",
    colors=["blue", "green"],
    saveFig=True,
    pdf=True
)

# Plot for task 2.5
# comparaison between philo , philo tas and philo tatas
plot_multiple_datasets(
    datasets=[philo_data, philo_data_TAS, philo_data_TATAS],
    labels=["Philo POSIX", "Philo TAS", "Philo TATAS"],
    title="Performance Philosophers",
    output_file="philo_comparaison",
    colors=["blue", "green", "red"],
    saveFig=True,
    pdf=True
)
# comparaison between rw , rw tas and rw tatas
plot_multiple_datasets(
    datasets=[rw_data, rw_data_TAS, rw_data_TATAS],
    labels=["RW POSIX", "RW TAS", "RW TATAS"],
    title="Performance Readers/Writers",
    output_file="rw_comparaison",
    colors=["blue", "green", "red"],
    saveFig=True,
    pdf=True
)
# comparaison between pc , pc tas and pc tatas
plot_multiple_datasets(
    datasets=[pc_data, pc_data_TAS, pc_data_TATAS],
    labels=["PC POSIX", "PC TAS", "PC TATAS"],
    title="Performance Producers/Consumers",
    output_file="pc_comparaison",
    colors=["blue", "green", "red"],
    saveFig=True,
    pdf=True
)