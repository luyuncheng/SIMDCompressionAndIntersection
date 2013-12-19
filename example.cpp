//
// A simple example to get you started with the library.
// You can compile and run this example like so:
//
//   make example
//   ./example
//
//  Warning: If your compiler does not fully support C++11, some of
//  this example may require changes.
//

#include "codecfactory.h"
#include "intersection.h"

int main() {
    // We pick a CODEC
    IntegerCODEC & codec =  * CODECFactory::getFromName("s-bp128-m");
    // could use others, e.g., "varint", "s-fastpfor-1"
    ////////////
    //
    // create a container with some integers in it
    //
    // We need the integers to be in sorted order.
    //
    // (Note: You don't need to use a vector.)
    //
    size_t N = 10 * 1000;
    vector<uint32_t> mydata(N);
    for(uint32_t i = 0; i < N;++i) mydata[i] = 3*i;
    ///////////
    //
    // You need some "output" container. You are responsible
    // for allocating enough memory.
    //
    vector<uint32_t> compressed_output(N+1024);
    // N+1024 should be plenty
    //
    //
    size_t compressedsize = compressed_output.size();
    codec.encodeArray(mydata.data(), mydata.size(),
            compressed_output.data(), compressedsize);
    //
    // if desired, shrink back the array:
    compressed_output.resize(compressedsize);
    compressed_output.shrink_to_fit();
    // display compression rate:
    cout<<setprecision(3);
    cout<<"You are using " << 32.0 * static_cast<double>(compressed_output.size()) /
                 static_cast<double>(mydata.size()) <<" bits per integer. "<<endl;
    //
    // You are done!... with the compression...
    //
    ///
    // decompressing is also easy:
    //
    vector<uint32_t> mydataback(N);
    size_t recoveredsize = mydataback.size();
    //
    codec.decodeArray(compressed_output.data(),
            compressed_output.size(), mydataback.data(), recoveredsize);
    mydataback.resize(recoveredsize);
    //
    // That's it for compression!
    //
    if(mydataback != mydata) throw runtime_error("bug!");

    //
    // Next we are going to test out intersection...
    //
    vector<uint32_t> mydata2(N);
    for(uint32_t i = 0; i < N;++i) mydata2[i] = 6*i;
    intersectionfunction  inter = IntersectionFactory::getFromName("simd");// using SIMD intersection
    //
    // we are going to intersect mydata and mydata2 and write back
    // the result to mydata2
    //
    size_t intersize = inter(mydata2.data(),mydata2.size(),mydata.data(),mydata.size(),mydata2.data());
    mydata2.resize(intersize);
    mydata2.shrink_to_fit();
    cout<<"Intersection size: " << mydata2.size() <<"  integers. "<<endl;
}
