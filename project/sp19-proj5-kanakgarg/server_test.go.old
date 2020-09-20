package main

import (
	"bytes"
	"github.com/61c-teach/sp19-proj5-userlib"
	"net/http"
	"net/url"
	"testing"
)

/*
 *	This is a hacky method to make the handler think that it is talking to the web response.
 */
type ResponseWriterTester struct {
	http.ResponseWriter
	header http.Header
	data []byte
	statusCode int
}

func (r *ResponseWriterTester) Header() http.Header {
	return r.header
}

func (r *ResponseWriterTester) Write(d []byte) (int, error) {
	r.data = append(r.data, d...)
	return len(d), nil
}

func (r *ResponseWriterTester) WriteHeader(statusCode int) {
	r.statusCode = statusCode
}

func TestBasicFileTest(t *testing.T) {
	// We first need to define a capacity and timeout so our cache has some parameters.
	capacity = 1000
	timeout = 2
	workingDir = ""
	// We then need to launch the cache since we are not directly calling main to do this testing.
	go operateCache()
	// We need to set up a response writer which will be the dummy passed into the handler to make testing easier.
	resp := ResponseWriterTester{header: http.Header{}}
	// This is the filename which will be passed into the handler as a url.
	name := "/cs61c.html"
	// For this test, we expect the only change to the file name to be adding a dot in front of it since there is nothing else to replace.
	expectedName := "." + name
	// Here I take the name and turn it into a URL to use later.
	path := url.URL{Path:name}
	// I keep a dummy readName variable which will be set by the custom userlib function I defined below.
	readName := ""
	// This is the data which that fake file will contain.
	DataToBeRead := []byte("CS61C is the best class in the world! Emperor Nick shall reign supreme.")
	// This is bad data which will be read if the filename is not correct.
	badData := []byte("CS61C is the worst class ever!")
	// We finally make the http request which the handler can understand.
	req := http.Request{URL:&path}
	// We set the userlib FileRead function to this custom 'read'.
	userlib.ReplaceReadFile(func(workingDir, filename string)(data []byte, err error){
		// Set the global readName variable to what we read for an error check later.
		readName = filename
		// If we get the expected name, we will return the correct data.
		if filename == expectedName {
			data = DataToBeRead
		} else {
			// Otherwise we return the bad data.
			data = badData
		}
		return
	})
	// We finally will call the handler. This is where we will get the data from the cache and or filesystem depending
	// on if it is in the cache. It will be stored in the dummy Response Writer which was created above.
	handler(&resp, &req)
	// Finally we validate if we read the correct filename.
	if expectedName != readName {
		t.Errorf("The path (%s) which was passed in was not correct!", readName)
	}
	// We will also assert that we have read the correct bytes just to be certain that everything was saved and forwarded correctly.
	if !bytes.Equal(DataToBeRead, resp.data) {
		t.Errorf("The data that was received (%s) was not what was expected (%s)!", string(resp.data), string(DataToBeRead))
	}
	// Next we check to see if the header has the correct value for the context type.
	if resp.header.Get(userlib.ContextType) != userlib.GetContentType(name){
		t.Errorf("Received the wrong context type for the file.")
	}
	// We finally wanna check that we set the correct return code.
	if resp.statusCode != userlib.SUCCESSCODE {
		t.Errorf("Received the wrong status code! Expected %v got %v", userlib.SUCCESSCODE, resp.statusCode)
	}
}