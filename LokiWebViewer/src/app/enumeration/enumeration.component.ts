import { Component } from '@angular/core';
import  { ServerService } from '../server.service';
import { HttpClient } from '@angular/common/http';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-enumeration',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './enumeration.component.html',
  styleUrl: './enumeration.component.css'
})


export class EnumerationComponent {
  SEVER_URL = this.serverService.getUrl();
  enumerateEndpoint = this.SEVER_URL + '/Action/Enum';
  resultEndpoint = this.SEVER_URL + '/Action/EnumResult';
  enumerateData = '';
  id: number = -1;
  result: string = '';
  pcName: string = '';
  osName: string = '';
  osVersion: string = '';
  osArchitecture: string = '';
  ram: string = '';
  cpuCores: string = '';
  mac: string = '';
  users: string[] = [] as string[];
  baseboard: string = '';
  diskSize: string = '';
  diskFree: string = '';


  constructor(private serverService: ServerService, private http: HttpClient) {
    // the id is passed in the URL
    this.id = parseInt(window.location.href.split('/').pop() || '-1');
    this.enumerate();
    this.getEnumerateData();
    console.log("PCName: " + this.pcName)
      console.log("OSName: " + this.osName)
      console.log("OSVersion: " + this.osVersion)
      console.log("OSArchitecture: " + this.osArchitecture)
      console.log("RAM: " + this.ram)
      console.log("CPU Cores: " + this.cpuCores)
      console.log("MAC: " + this.mac)
      console.log("Users: " + this.users)
      console.log("Baseboard: " + this.baseboard)
      console.log("Disk Size " + this.diskSize)
      console.log("Desk Free" + this.diskFree);
  }

  enumerate() {
    this.http.post(this.enumerateEndpoint, {'id' : this.id.toString() }).subscribe((data: any) => {
      console.log(data);
      this.enumerateData = data;
    });
  }

  closePopup() {
    // redirect to the main page
    window.location.href = "/";
  }

  getEnumerateData() {
    this.http.post(this.resultEndpoint, {'id' : this.id.toString() }).subscribe((data: any) => {
      console.log(data);
      this.result = data;
        // PCName:MALDEV!OSName: Windows!OSVersion: Error1150!OSArchitecture: 64!RAM: 9071MB!CPU Cores: 6!MAC: 08-00-27-20-90-ef!Users: Administrator, b, DefaultAccount, Guest, WDAGUtilityAccount, !Baseboard: MALDEV!Disk Size: 49 GB!Disk Free: 0 GB!"
        const parts = data.output.toString().split('!') || [];
        console.log(parts);        
        this.pcName = parts[0].split(':')[1];
        this.osName = parts[1].split(':')[1];
        this.osVersion = parts[2].split(':')[1];
        this.osArchitecture = parts[3].split(':')[1];
        this.ram = parts[4].split(':')[1];
        this.cpuCores = parts[5].split(':')[1];
        this.mac = parts[6].split(':')[1];
        this.users = parts[7].split(':')[1].split(', ');
        // remove the last empty string
        this.users.pop();
        this.baseboard = parts[8].split(':')[1];
        this.diskSize = parts[9].split(':')[1];
        this.diskFree = parts[10].split(':')[1];
    });
  }


}
