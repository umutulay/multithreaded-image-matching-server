#include "../include/client.h"

int port = 0;

pthread_t worker_thread[100];
int worker_thread_id = 0;
char output_path[1028];

processing_args_t req_entries[100];

/* TODO: implement the request_handle function to send the image to the server and recieve the processed image
* 1. Open the file in the read-binary mode (i.e. "rb" mode) - Intermediate Submission
* 2. Get the file length using the fseek and ftell functions - Intermediate Submission
* 3. set up the connection with the server using the setup_connection(int port) function - Intermediate Submission
* 4. Send the file to the server using the send_file_to_server(int fd, FILE *file, int size) function - Intermediate Submission
* 5. Receive the processed image from the server using the receive_file_from_server(int socket, char *file_path) function
* 6. receive_file_from_server saves the processed image in the output directory, so pass in the right directory path
* 7. Close the file
*/
void * request_handle(void * img_file_path)
{
    FILE* fptr;
    if ((fptr = fopen((char*)img_file_path, "rb")) == NULL) {
        perror("fopen error");
        fclose(fptr);
        return (void *) 1;
        
    }

    if (fseek(fptr, 0, SEEK_END) != 0) {
        perror("fseek error");
        fclose(fptr);
        return (void *) 1;
    }

    int size = ftell(fptr);
    if (size == -1) {
        perror("ftell error");
        fclose(fptr);
        return (void *) 1;
    }

    rewind(fptr);


    int server_fd = setup_connection(port);

    if (server_fd < 0) {
        fprintf(stderr, "Failed to set up server connection\n");
        fclose(fptr);
        return (void *) 1;
    }

    if (send_file_to_server(server_fd, fptr, size) < 0) {
        fprintf(stderr, "Failed to send file to server\n");
        close(server_fd);
        fclose(fptr);
        return (void *) 1;
    }

    char buffer [1000];
    sprintf(buffer, "output/%s", (char*)img_file_path+4);
    if (receive_file_from_server(server_fd, buffer) < 0) {
        fprintf(stderr, "Failed to receive\n");
        close(server_fd);
        fclose(fptr);
        return (void *) 1;
    }

    fclose(fptr);
    return (void *) 1;
}

/* Directory traversal function is provided to you. */
void directory_trav(char * img_directory_path)
{
    char dir_path[BUFF_SIZE]; 
    strcpy(dir_path, img_directory_path);
    struct dirent *entry; 
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("Opendir ERROR");
        exit(0);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".DS_Store") != 0)
        {
            sprintf(req_entries[worker_thread_id].file_name, "%s/%s", dir_path, entry->d_name);
            printf("New path: %s\n", req_entries[worker_thread_id].file_name);
            pthread_create(&worker_thread[worker_thread_id], NULL, request_handle, (void *) req_entries[worker_thread_id].file_name);
            worker_thread_id++;
        }
    }
    closedir(dir);
    for(int i = 0; i < worker_thread_id; i++)
    {
        pthread_join(worker_thread[i], NULL);
    }
}


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: ./client <directory path> <Server Port> <output path>\n");
        exit(-1);
    }
    /*TODO:  Intermediate Submission
    * 1. Get the input args --> (1) directory path (2) Server Port (3) output path
    */
    char *directory_path = argv[1];
    port = atoi(argv[2]);
    strcpy(output_path, argv[3]);
    /*TODO: Intermediate Submission
    * Call the directory_trav function to traverse the directory and send the images to the server
    */

    directory_trav(directory_path);

    return 0;  
}