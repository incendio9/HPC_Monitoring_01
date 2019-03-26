#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Init_NodeMemLoad() {

    sysinfo(&memInfo);

    MLLayout = new QVBoxLayout;
    MLChart = new QCustomPlot[NodeCount];
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    nodelist *curr_node = HPCNodes;

    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        MLChart[i].addGraph();
        MLChart[i].graph(0)->setPen(QPen(QColor(255, 0, 0)));
        MLChart[i].xAxis->setLabelFont(QFont("Consolas", 10, 0,false));
        MLChart[i].xAxis->setLabel(curr_node->NodeName);
        MLChart[i].xAxis->setLabelPadding(5);
        MLChart[i].xAxis->setTicker(timeTicker);
        MLChart[i].axisRect()->setupFullAxesBox();
        MLChart[i].yAxis->setRange(0, 100);
        MLChart[i].setMinimumHeight(200);
        MLLayout->addWidget(&MLChart[i]);
    }

    ui->scrollAreaWidgetContents_2->setLayout(MLLayout);

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

    MLProcess = new QProcess(this);
    MLCommand = "sbatch";
    QString MLScript = "/nfs/HPC/WORK/mem_load.slurm";
    MLArguments << MLScript;
    MLProcess->start(MLCommand, MLArguments);
    MLProcess->waitForFinished();

    //printf("Init_MemLoadOverview\n");
    return 0;

}
