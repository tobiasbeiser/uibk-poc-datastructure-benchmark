import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

# Check if the correct number of arguments is provided
if len(sys.argv) != 2:
    print("Usage: python script.py <path_to_csv_file>")
    sys.exit(1)

# Get the CSV file path from the command-line argument
csv_file_path = sys.argv[1]

# Create the results directory if it does not exist
os.makedirs('results', exist_ok=True)

# Read the CSV file
data = pd.read_csv(csv_file_path, delimiter=';')

# Translate element size for the plot title
element_size_translation = {
    1: '8 Byte',
    2: '512 Byte',
    3: '8 MB'
}

# Get unique parameter combinations
unique_params = data[['elementSize', 'collectionSize', 'Read/Writes', 'Insert/Deletes']].drop_duplicates()

for _, params in unique_params.iterrows():
    # Filter data for the current set of parameters
    filtered_data = data[
        (data['elementSize'] == params['elementSize']) &
        (data['collectionSize'] == params['collectionSize']) &
        (data['Read/Writes'] == params['Read/Writes']) &
        (data['Insert/Deletes'] == params['Insert/Deletes'])
    ]

    # Create the plot
    plt.figure(figsize=(14, 10))
    plt.barh(filtered_data['collectionName'], filtered_data['TotalOperations/Second'], color='skyblue')
    plt.xlabel('TotalOperations/Second')
    plt.ylabel('Data Structures')
    plt.xscale('log')
    
    element_size_text = element_size_translation[params['elementSize']]
    
    plt.title(f'Performance of Different Data Structures\n'
              f'elementSize={element_size_text}, collectionSize={params["collectionSize"]}, '
              f'Read/Writes={params["Read/Writes"]}%, Insert/Deletes={params["Insert/Deletes"]}%')

    # Use tight_layout to adjust subplot parameters for a nicer fit
    plt.tight_layout()

    # Save the plot
    image_name = (f'result_{params["elementSize"]}_{params["collectionSize"]}_'
                  f'{params["Read/Writes"]}_{params["Insert/Deletes"]}.png')
    plt.savefig(os.path.join('results', image_name))
    plt.close()

print("Plots have been generated and saved in the 'results' directory.")
