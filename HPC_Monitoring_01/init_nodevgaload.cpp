#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Init_NodeVGALoad() {

    GPULoadLayout = new QVBoxLayout;
    MemLoadLayout = new QVBoxLayout;
    VLChartLayout_GPU = new QGridLayout[NodeCount];
    VLChartLayout_Mem = new QGridLayout[NodeCount];
    VLChart_GPU = new QCustomPlot*[NodeCount];
    VLChart_Mem = new QCustomPlot*[NodeCount];

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    nodelist *curr_node = HPCNodes;

    for (int i = 0; i < NodeCount; i++) {

        curr_node = curr_node->next_node;

        if (curr_node->VGACount > 0) {

            VLChart_GPU[i] = new QCustomPlot[curr_node->VGACount];
            VLChart_Mem[i] = new QCustomPlot[curr_node->VGACount];

            for (int j = 0; j < curr_node->VGACount; j++) {

                VLChart_GPU[i][j].addGraph();
                VLChart_GPU[i][j].graph(0)->setPen(QPen(QColor(255, 0, 0)));
                VLChart_GPU[i][j].xAxis->setLabelFont(QFont("Consolas", 10, 0,false));
                VLChart_GPU[i][j].xAxis->setLabel(curr_node->NodeName);
                VLChart_GPU[i][j].xAxis->setLabelPadding(5);
                VLChart_GPU[i][j].xAxis->setTicker(timeTicker);
                VLChart_GPU[i][j].axisRect()->setupFullAxesBox();
                VLChart_GPU[i][j].yAxis->setRange(0, 100);
                VLChart_GPU[i][j].setMinimumHeight(200);

                VLChart_Mem[i][j].addGraph();
                VLChart_Mem[i][j].graph(0)->setPen(QPen(QColor(255, 0, 0)));
                VLChart_Mem[i][j].xAxis->setLabelFont(QFont("Consolas", 10, 0,false));
                VLChart_Mem[i][j].xAxis->setLabel(curr_node->NodeName);
                VLChart_Mem[i][j].xAxis->setLabelPadding(5);
                VLChart_Mem[i][j].xAxis->setTicker(timeTicker);
                VLChart_Mem[i][j].axisRect()->setupFullAxesBox();
                VLChart_Mem[i][j].yAxis->setRange(0, 100);
                VLChart_Mem[i][j].setMinimumHeight(200);

                VLChartLayout_GPU[i].addWidget(&VLChart_GPU[i][j]);
                VLChartLayout_Mem[i].addWidget(&VLChart_Mem[i][j]);

            }

            GPULoadLayout->addLayout(&VLChartLayout_GPU[i]);
            MemLoadLayout->addLayout(&VLChartLayout_Mem[i]);

        }

    }

    ui->scrollAreaWidgetContents_3->setLayout(GPULoadLayout);
    ui->scrollAreaWidgetContents_4->setLayout(MemLoadLayout);

    curr_node = HPCNodes;
    int cnt_vga_node = 0;
    FILE *vga_load_slurm = fopen("/nfs/HPC/WORK/vga_load.slurm", "w");
    fprintf(vga_load_slurm, "#!/bin/bash\n");
    fprintf(vga_load_slurm, "\n");
    fprintf(vga_load_slurm, "#SBATCH -v\n");
    fprintf(vga_load_slurm, "#SBATCH --nodelist=");
    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        if ( curr_node->VGACount > 0 ){
            fprintf(vga_load_slurm, "%s", curr_node->NodeName);
            cnt_vga_node++;
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

    VLProcess = new QProcess(this);
    VLCommand = "sbatch";
    QString VLScript = "/nfs/HPC/WORK/vga_load.slurm";
    VLArguments << VLScript;
    VLProcess->start(VLCommand, VLArguments);
    VLProcess->waitForFinished();

    return 0;

}
