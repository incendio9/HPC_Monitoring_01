#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Update_NodeVGALoad(double key) {

    nodelist *curr_node = HPCNodes;

    if (curr_node->VGACount > 0) {

        nvmlInit();

        nvmlDevice_t device;
        nvmlUtilization_t device_utilization;

        float gpuavg = 0.0;
        float memavg = 0.0;


        for (int i = 0; i < curr_node->VGACount; i++) {
            nvmlDeviceGetHandleByIndex(i, &device);
            nvmlDeviceGetUtilizationRates(device, &device_utilization);
            curr_node->VGALoad_GPU[i] = device_utilization.gpu;
            curr_node->VGALoad_Mem[i] = device_utilization.memory;
            gpuavg += (float)curr_node->VGALoad_GPU[i];
            memavg += (float)curr_node->VGALoad_Mem[i];
        }

        nvmlShutdown();

        gpuavg = gpuavg / curr_node->VGACount;
        memavg = memavg / curr_node->VGACount;
        QString MasterVGPULoad;
        QString MasterVMemLoad;

        ui->label_12->setText(MasterVGPULoad.sprintf("%.2f", gpuavg));
        ui->label_14->setText(MasterVMemLoad.sprintf("%.2f", memavg));

    }

    FILE *gpuload = fopen("/nfs/HPC/WORK/vga_load.out", "r");
    char *string = new char[1024];
    char *temp = new char[1024];
    char *token;
    int device_num = 0;
    QString label;

    while (fgets(string, 1024, gpuload) != nullptr) {

        strcpy(temp, string);
        token = strtok(temp, " ");

        curr_node = HPCNodes;

        for (int i = 0; i < NodeCount; i++) {

            curr_node = curr_node->next_node;

            if (strncmp(token, curr_node->NodeName, strlen(curr_node->NodeName)) == 0) {

                token = strtok(nullptr, " \n");
                device_num = atoi(token);

                for (int j = 0; j < device_num; j++) {

                    token = strtok(nullptr, " \n");
                    curr_node->VGALoad_GPU[j] = (unsigned int)atoi(token);
                    VLChart_GPU[i][j].graph(0)->addData(key, (double)curr_node->VGALoad_GPU[j]);
                    VLChart_GPU[i][j].xAxis->setRange(key, 8, Qt::AlignRight);
                    label.sprintf("%s GPU #%d : %u%%", curr_node->NodeName, j, curr_node->VGALoad_GPU[j]);
                    VLChart_GPU[i][j].xAxis->setLabel(label);
                    VLChart_GPU[i][j].replot();

                    token = strtok(nullptr, " \n");
                    curr_node->VGALoad_Mem[j] = (unsigned int)atoi(token);
                    VLChart_Mem[i][j].graph(0)->addData(key, (double)curr_node->VGALoad_Mem[j]);
                    VLChart_Mem[i][j].xAxis->setRange(key, 8, Qt::AlignRight);
                    label.sprintf("%s GPU #%d : %u%%", curr_node->NodeName, j, curr_node->VGALoad_Mem[j]);
                    VLChart_Mem[i][j].xAxis->setLabel(label);
                    VLChart_Mem[i][j].replot();

                }

                break;

            }

        }

    }

    fclose(gpuload);
    delete[] string;
    delete[] temp;

    curr_node = HPCNodes;
    int cnt_vga_node = 0;
    FILE *vga_load_slurm = fopen("/nfs/HPC/WORK/vga_load.slurm", "w");
    fprintf(vga_load_slurm, "#!/bin/bash\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "#SBATCH -v\n");
    fprintf(vga_load_slurm, "#SBATCH --nodelist=");
    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        if (curr_node->VGACount > 0) {
            if ( (strcmp(curr_node->NodeStatus, "IDLE") == 0)
                 || (strcmp(curr_node->NodeStatus, "ALLOCATED") == 0) ){
                fprintf(vga_load_slurm, "%s", curr_node->NodeName);
                cnt_vga_node++;
            }
            if (cnt_vga_node < NumVGANodes) fprintf(vga_load_slurm, ",");
            else {
                fprintf(vga_load_slurm, "\n");
                break;
            }
        }
    }
    fprintf(vga_load_slurm, "#SBATCH --ntasks=%d\n", NumVGANodes);
    fprintf(vga_load_slurm, "#SBATCH --time=1:00:00\n");
    fprintf(vga_load_slurm, "#SBATCH -o /nfs/HPC/WORK/vga_load.out\n");
    fprintf(vga_load_slurm, "#SBATCH -D /nfs/HPC/WORK/\n");
    fprintf(vga_load_slurm, "#SBATCH --export=NONE\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "mycommand=\"/nfs/HPC/WORK/vga_load\"\n");
    fprintf(vga_load_slurm, "myargs=\"\"\n");
    fprintf(vga_load_slurm, "infiles=(\"\")\n");
    fprintf(vga_load_slurm, "outfiles=(\"\")\n");
    fprintf(vga_load_slurm, "mydir=\"/nfs/HPC/WORK/\"\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "for file in ${infiles[@]}; do\n");
    fprintf(vga_load_slurm, "\tcp $file $mydir\n");
    fprintf(vga_load_slurm, "done\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "srun --ntasks=${SLURM_NTASKS} --mpi=pmi2 $mycommand $myargs\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "ret=$?\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "cd \"$mydir\" || exit 1\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "exit $ret\n");
    fclose(vga_load_slurm);

    return 0;

}
