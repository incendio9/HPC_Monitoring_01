#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Update_NodeMemLoad(double key) {

    totalPhysMem = memInfo.totalram;
    totalPhysMem *= memInfo.mem_unit;

    physMemUsed = memInfo.totalram - memInfo.freeram;
    physMemUsed *= memInfo.mem_unit;

    nodelist *curr_node = HPCNodes;
    curr_node->MEMLoad = (float)physMemUsed / (float)totalPhysMem * 100.0f;
    QString mastermemload;
    ui->label_10->setText(mastermemload.sprintf("%.2f", curr_node->MEMLoad));

    FILE *memload = fopen("/nfs/HPC/WORK/mem_load.out", "r");
    char *string = new char[1024];
    char *temp = new char[1024];
    char *token;
    QString label;
    while (fgets(string, 1024, memload) != nullptr) {
        strcpy(temp, string);
        token = strtok(temp, " ");
        curr_node = HPCNodes;
        for (int i = 0; i < NodeCount; i++) {
            curr_node = curr_node->next_node;
            if (strncmp(token, curr_node->NodeName, strlen(curr_node->NodeName)) == 0) {
                token = strtok(nullptr, " \n");
                curr_node->MEMLoad = atof(token);
                MLChart[i].graph(0)->addData(key, (double)curr_node->MEMLoad);
                MLChart[i].xAxis->setRange(key, 8, Qt::AlignRight);
                label.sprintf("%s : %.2f", curr_node->NodeName, curr_node->MEMLoad);
                MLChart[i].xAxis->setLabel(label);
                MLChart[i].replot();
                break;
            }
        }
    }

    fclose(memload);
    delete[] string;
    delete[] temp;

    curr_node = HPCNodes;
    int cnt_avail_node = 0;
    FILE *mem_load_slurm = fopen("/nfs/HPC/WORK/mem_load.slurm", "w");
    fprintf(mem_load_slurm, "#!/bin/bash\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "#SBATCH -v\n");
    fprintf(mem_load_slurm, "#SBATCH --nodelist=");
    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        if ( (strcmp(curr_node->NodeStatus, "IDLE") == 0)
             || (strcmp(curr_node->NodeStatus, "ALLOCATED") == 0) ){
            fprintf(mem_load_slurm, "%s", curr_node->NodeName);
            cnt_avail_node++;
            if (cnt_avail_node < NumAvailNodes) fprintf(mem_load_slurm, ",");
            else {
                fprintf(mem_load_slurm, "\n");
                break;
            }
        }
    }
    fprintf(mem_load_slurm, "#SBATCH --ntasks=%d\n", NumAvailNodes);
    fprintf(mem_load_slurm, "#SBATCH --time=1:00:00\n");
    fprintf(mem_load_slurm, "#SBATCH -o /nfs/HPC/WORK/mem_load.out\n");
    fprintf(mem_load_slurm, "#SBATCH -D /nfs/HPC/WORK/\n");
    fprintf(mem_load_slurm, "#SBATCH --export=NONE\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "mycommand=\"/nfs/HPC/WORK/mem_load\"\n");
    fprintf(mem_load_slurm, "myargs=\"\"\n");
    fprintf(mem_load_slurm, "infiles=(\"\")\n");
    fprintf(mem_load_slurm, "outfiles=(\"\")\n");
    fprintf(mem_load_slurm, "mydir=\"/nfs/HPC/WORK/\"\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "for file in ${infiles[@]}; do\n");
    fprintf(mem_load_slurm, "\tcp $file $mydir\n");
    fprintf(mem_load_slurm, "done\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "srun --ntasks=${SLURM_NTASKS} --mpi=pmi2 $mycommand $myargs\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "ret=$?\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(mem_load_slurm, "\n");
    fprintf(mem_load_slurm, "exit $ret\n");
    fclose(mem_load_slurm);

    return 0;

}
