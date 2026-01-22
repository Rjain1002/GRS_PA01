# Used AI to generate a plotting script for MT25080 Part D based on provided CSV data.
import matplotlib.pyplot as plt
import pandas as pd
import os

# Files
CSV_C = "MT25080_Part_C_CSV.csv"
CSV_D = "MT25080_Part_D_CSV.csv"

def plot_part_c():
    if not os.path.exists(CSV_C):
        print(f"File {CSV_C} not found.")
        return

    try:
        df = pd.read_csv(CSV_C)
    except Exception as e:
        print(f"Error reading {CSV_C}: {e}")
        return

    # Split Program+Function into Program and Worker
    df[['Program', 'Worker']] = df['Program+Function'].str.split('+', expand=True)

    metrics = ['ExecTime', 'CPU%', 'Mem', 'IO']
    titles = ['Execution Time (s)', 'CPU Usage (%)', 'Memory Usage (%)', 'IO Usage']

    fig, axes = plt.subplots(2, 2, figsize=(15, 10))
    axes = axes.flatten()

    colors = ['skyblue', 'lightcoral', 'lightgreen', 'gold', 'violet', 'orange']

    for i, (col, title) in enumerate(zip(metrics, titles)):
        bars = axes[i].bar(df['Program+Function'], df[col], color=colors[:len(df)])
        axes[i].set_title(f'{title} Comparison', fontsize=14)
        axes[i].set_xlabel('Program + Worker', fontsize=12)
        axes[i].set_ylabel(title, fontsize=12)
        axes[i].grid(axis='y', linestyle='--', alpha=0.7)
        axes[i].tick_params(axis='x', rotation=45)

        # Add value labels on bars
        for bar in bars:
            height = bar.get_height()
            axes[i].text(bar.get_x() + bar.get_width()/2., height + max(df[col])*0.01,
                        f'{height:.2f}', ha='center', va='bottom', fontsize=10)

    plt.tight_layout()
    plt.savefig('plot_part_c_combined.png', dpi=300)
    plt.close()
    print("Generated plot_part_c_combined.png")

def plot_part_d():
    if not os.path.exists(CSV_D):
        print(f"File {CSV_D} not found.")
        return
    
    try:
        df = pd.read_csv(CSV_D)
    except Exception as e:
        print(f"Error reading {CSV_D}: {e}")
        return
    
    # Split Program+Function into Program and Worker
    df[['Program', 'Worker']] = df['Program+Function'].str.split('+', expand=True)

    # Separate Prog A (Processes) and Prog B (Threads)
    df_a = df[df['Program'].str.contains('Program_A')]
    df_b = df[df['Program'].str.contains('Program_B')]

    # Plot for each metric with subplots for workers
    metrics = ['CPU%', 'Mem', 'IO', 'ExecTime']
    workers = ['cpu', 'mem', 'io']

    for metric in metrics:
        fig, axes = plt.subplots(1, 3, figsize=(18, 6))
        
        for i, worker in enumerate(workers):
            df_a_worker = df_a[df_a['Worker'] == worker]
            df_b_worker = df_b[df_b['Worker'] == worker]

            axes[i].plot(df_a_worker['Count'], df_a_worker[metric], marker='o', linestyle='-', color='blue', label='Program A (Processes)')
            axes[i].plot(df_b_worker['Count'], df_b_worker[metric], marker='s', linestyle='--', color='red', label='Program B (Threads)')
            axes[i].set_title(f'{worker.upper()} Intensive Worker', fontsize=14)
            axes[i].set_xlabel('Number of Workers', fontsize=12)
            axes[i].set_ylabel(f'{metric} Usage' + (' (%)' if metric != 'ExecTime' else ' (s)'), fontsize=12)
            axes[i].grid(True, alpha=0.5)
            axes[i].legend(fontsize=10)

        plt.suptitle(f'Part D: {metric} Scaling - Processes vs Threads', fontsize=16)
        plt.tight_layout()
        plt.savefig(f'plot_part_d_{metric.lower()}_scaling.png', dpi=300)
        plt.close()
        print(f"Generated plot_part_d_{metric.lower()}_scaling.png")

if __name__ == "__main__":
    print("Starting Plot Generation...")
    plot_part_c()
    plot_part_d()
    print("Plot Generation Complete.")
