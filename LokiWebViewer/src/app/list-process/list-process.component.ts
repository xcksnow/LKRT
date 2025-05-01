import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { CommonModule } from '@angular/common';
import { NgxPaginationModule } from 'ngx-pagination';
import { FormsModule } from '@angular/forms';
import { ServerService } from '../server.service';

export interface PagingConfig {
  currentPage: number;
  itemsPerPage: number;
  totalItems: number;
}

interface Process {
  Name: string;
  PID: string;
  ParentPID: string;
  Username: string;
}

@Component({
  selector: 'app-list-process',
  standalone: true,
  imports: [CommonModule, NgxPaginationModule, FormsModule],
  templateUrl: './list-process.component.html',
  styleUrls: ['./list-process.component.css']
})

export class ListProcessComponent {
  processes: Process[] = [];
  processesToShow: Process[] = [];
  id: string = '';
  SERVER_URL = this.serverService.getUrl();
  isLoading: boolean = false;
  endpoint = `${this.SERVER_URL}/Action/Processes`;
  getEndpoint = `${this.SERVER_URL}/Process/Get`;
  totalItems: number = 0;
  pageSize: number = 18;
  currentPageR: number = 1;
  tableSize: number[] = [5, 10, 15, 20];
  pagingConfig: PagingConfig = {} as PagingConfig;
  processFilter: string = '';

  constructor(private http: HttpClient, private serverService: ServerService) {
    this.isLoading = true;
    this.pagingConfig = {
      itemsPerPage: this.pageSize,
      currentPage: this.currentPageR,
      totalItems: this.totalItems
    };
  }

  ngOnInit() {
    this.id = window.location.href.split('/').pop() || '';
    if (this.id !== '') {
      console.log('id: ' + this.id);
      this.getProcesses();
    }
    console.log('ListProcessComponent initialized');
  }

  homePage(){
    window.location.href = '';
  }

  getProcesses() {
    this.http.post(this.endpoint, { id: this.id }).subscribe((data: any) => {
      console.log(data);
    });
  
    setTimeout(() => {
      this.http.post(this.getEndpoint, { id: this.id }).subscribe((data: any) => {
        console.log(data);
        // Split the output by '!' to get individual processes
        data.output.split('!').forEach((processString: string) => {
          // Split the process string by ',' to get individual properties
          const processArray = processString.split(',');
          // Check if the process array has enough elements
          if (processArray.length >= 5) {
            // Create a new Process object
            const newProcess: Process = {
              Name: processArray[0].trim().split(':')[1].trim(),
              PID: processArray[1].trim().split(':')[1].trim(),
              ParentPID: processArray[2].trim().split(':')[1].trim(),
              Username: processArray[3].trim().split(':')[1].trim(),
            };
            // Push the new Process object into the processes array
            this.processes.push(newProcess);
            this.processesToShow.push(newProcess);
            // delete the duplicate (same PID) processes
            this.processes = this.processes.filter((process, index, self) =>
              index === self.findIndex((t) => (
                t.PID === process.PID
              ))
            );
            this.processesToShow = this.processesToShow.filter((process, index, self) =>
              index === self.findIndex((t) => (
                t.PID === process.PID
              ))
            );
          } else {
            console.error('Invalid process format:', processString);
          }
        });
        console.log(this.processes); 
        this.pagingConfig.totalItems = this.processes.length;
        this.isLoading = false;
      });
    }, 8000); 
  }

  changeFilter() {
    console.log(this.processFilter);
    this.processesToShow = this.processes.filter((process: Process) => {
      return process.Name.toLowerCase().includes(this.processFilter.toLowerCase()) ||
             process.PID.toLowerCase().includes(this.processFilter.toLowerCase()) ||
             process.ParentPID.toLowerCase().includes(this.processFilter.toLowerCase()) ||
             process.Username.toLowerCase().includes(this.processFilter.toLowerCase());
    }
    );
    this.pagingConfig.totalItems = this.processes.length;
  }
  
}
