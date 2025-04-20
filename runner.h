/**
 * @brief Code that interprets the actual http response from the server
 * and turns it into an actual programming language!!!
 * 
 * @author: Jace Parks <andrewID: jacep>
 * @date: 4/20/2025
 */

struct runtime {
    long *playground; //Array of values to be changed
    
    struct nlist; //Hashmap of variables
};

struct responseParsed {
    int responseCode; //Response Code

};
typedef struct responseParsed ServerResponse;

/**
 * @brief Parses a string (server response) into a struct to
 * be read by the runtime thingy!
 */
ServerResponse* parseResponse(char *response);
