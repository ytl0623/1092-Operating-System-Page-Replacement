#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std ;

void FIFO( int pageFrame, string pageSchedule, fstream & outfile ) {  // First In First Out
  vector< char > buffer ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  outfile << "--------------FIFO-----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    int storedIndex ;
    bool isFault = false, stored = false ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        storedIndex = j ;
        stored = true ;
      }
    }

    if ( ! stored ) {
      isFault = true ;
      pageFault++ ;

      for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
        buffer[ j ] = buffer[ j - 1 ] ;  // move forward
      }

      buffer[ 0 ] = pageSchedule[ i ] ;
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' )
      outfile << buffer[ j ] ;
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl ;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl ;
}

void LRU( int pageFrame, string pageSchedule, fstream & outfile ) {  // Least Recently Used
  vector< char > buffer ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  outfile << "--------------LRU-----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    int storedIndex ;
    bool isFault = false, stored = false ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        storedIndex = j ;
        stored = true ;
      }
    }

    if ( stored ) {
      for ( int j = storedIndex ; j > 0 ; j-- ) {
        buffer[ j ] = buffer[ j - 1 ] ;
      }

      buffer[ 0 ] = pageSchedule[ i ] ;
    }
    else {
      isFault = true ;
      pageFault++ ;

      for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
        buffer[ j ] = buffer[ j - 1 ] ;
      }

      buffer[ 0 ] = pageSchedule[ i ] ;
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' ) {
        outfile << buffer[ j ] ;
      }
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl ;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl ;
}

void LFU_FIFO( int pageFrame, string pageSchedule, fstream & outfile ) {  // Least Recently Used + First In First Out
  vector< char > buffer ;
  int timeStamp[100], counter[100] ;
  bool inBuffer[100] ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  for ( int i = 0 ; i < 100 ; i++ ) {  // initialize
    timeStamp[ i ] = 0 ;
    counter[ i ] = 0 ;
    inBuffer[ i ] = false ;
  }

  outfile << "--------------Least Frequently Used Page Replacement-----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    bool isFault = false, stored = false, full = true ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {  // pageSchedule[i] in page table ?
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        stored = true ;
      }
    }

    if ( stored ) {  // Yes, pageSchedule[i] in page table
      counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
      // timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
    }
    else {  // page fault occur, 1. add new 2. replacement
      isFault = true ;
      pageFault++ ;

      for ( int j = 0 ; j < pageFrame ; j++ ) {
        if ( buffer[ j ] == '-' ) {
          full = false ;
          break ;
        }
      }

      if ( full ) {  // 2. replacement
        int minIndex = 0, minKey = 0, minCounter = 999999999, minTimestamp = 0 ;

        for ( int j = pageFrame - 1 ; j >= 0 ; j-- ) {  // check counter smallest
          if ( minCounter > counter[ ( int )buffer[ j ] - 48 ] ) {
            minCounter = counter[ ( int )buffer[ j ] - 48 ] ;
            minIndex = j ;
            minKey = ( int )buffer[ j ] - 48 ;
          }
        }

        minTimestamp = timeStamp[ minKey ] ;

        for ( int j = 0 ; j < pageFrame ; j++ ) {  // if counter as the same , check time stamp smallest
          if ( counter[ minKey ] == counter[ ( int )buffer[ j ] - 48] && timeStamp[ ( int )buffer[ j ] - 48 ] < minTimestamp && j != minIndex ) {
            minTimestamp = timeStamp[ ( int )buffer[ j ] - 48 ] ;
            minIndex = j ;
            minKey = ( int )buffer[ j ] - 48 ;
          }
        }

        timeStamp[ minKey ] = 0 ;
        counter[ minKey ] = 0 ;
        inBuffer[ minKey ] = false ;

        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
        timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;


        for ( int j = minIndex ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
      else {  // 1. add new
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;
        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;

        if ( counter[ ( int )pageSchedule[ i ] - 48 ] == 1 ) {
          timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        }

        for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' ) {
        outfile << buffer[ j ] ;
      }
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl ;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl ;
}

void MFU_FIFO( int pageFrame, string pageSchedule, fstream & outfile ) {  // Most Recently Used + First In First Out
  vector< char > buffer ;
  int timeStamp[100], counter[100] ;
  bool inBuffer[100] ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  for ( int i = 0 ; i < 100 ; i++ ) {
    timeStamp[ i ] = 0 ;
    counter[ i ] = 0 ;
    inBuffer[ i ] = false ;
  }

  outfile << "--------------Most Frequently Used Page Replacement -----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    bool isFault = false, stored = false, full = true ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        stored = true ;
      }
    }

    if ( stored ) {
      counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
      // timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
    }
    else {
      isFault = true ;
      pageFault++ ;

      for ( int j = 0 ; j < pageFrame ; j++ ) {
        if ( buffer[ j ] == '-' ) {
          full = false ;
          break ;
        }
      }

      if ( full ) {
        int maxIndex = 0, maxKey = 0, maxCounter = -999999999, maxTimestamp ;

        for ( int j = pageFrame - 1 ; j >= 0 ; j-- ) {  // check counter smallest
          if ( maxCounter < counter[ ( int )buffer[ j ] - 48 ] ) {
            maxCounter = counter[ ( int )buffer[ j ] - 48 ] ;
            maxIndex = j ;
            maxKey = ( int )buffer[ j ] - 48 ;
          }
        }

        maxTimestamp = timeStamp[ maxKey ] ;

        for ( int j = 0 ; j < pageFrame ; j++ ) {  // if counter as the same , check time stamp smallest
          if ( counter[ maxKey ] == counter[ ( int )buffer[ j ] - 48 ] && timeStamp[ ( int )buffer[ j ] - 48 ] < maxTimestamp && j != maxIndex ) {
            maxTimestamp = timeStamp[ ( int )buffer[ j ] - 48 ] ;
            maxIndex = j ;
            maxKey = ( int )buffer[ j ] - 48 ;
          }
        }

        timeStamp[ maxKey ] = 0 ;
        counter[ maxKey ] = 0 ;
        inBuffer[ maxKey ] = false ;

        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
        timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;

        for ( int j = maxIndex ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
      else {
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;
        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;\

        if ( counter[ ( int )pageSchedule[ i ] - 48 ] == 1 ) {
          timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        }

        for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' ) {
        outfile << buffer[ j ] ;
      }
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl;
}

void LFU_LRU( int pageFrame, string pageSchedule, fstream & outfile ) {  // Least Recently Used + Least Recently Used
  vector< char > buffer ;
  int timeStamp[100], counter[100] ;
  bool inBuffer[100] ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  for ( int i = 0 ; i < 100 ; i++ ) {  // initialize
    timeStamp[ i ] = 0 ;
    counter[ i ] = 0 ;
    inBuffer[ i ] = false ;
  }

  outfile << "--------------Least Frequently Used LRU Page Replacement-----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    int storedIndex = 0 ;
    bool isFault = false, stored = false, full = true ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {  // pageSchedule[i] in page table ?
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        storedIndex = j ;
        stored = true ;
      }
    }

    if ( stored ) {  // Yes, pageSchedule[i] in page table
      counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
      timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;

      for ( int j = storedIndex ; j > 0 ; j-- ) {
        buffer[ j ] = buffer[ j - 1 ] ;
      }

      buffer[ 0 ] = pageSchedule[ i ] ;

    }
    else {  // page fault occur, 1. add new 2. replacement
      isFault = true ;
      pageFault++ ;

      for ( int j = 0 ; j < pageFrame ; j++ ) {
        if ( buffer[ j ] == '-' ) {
          full = false ;
          break ;
        }
      }

      if ( full ) {  // 2. replacement
        int minIndex = 0, minKey = 0, minCounter = 999999999, minTimestamp = 0 ;

        for ( int j = pageFrame - 1 ; j >= 0 ; j-- ) {  // check counter smallest
          if ( minCounter > counter[ ( int )buffer[ j ] - 48 ] ) {
            minCounter = counter[ ( int )buffer[ j ] - 48 ] ;
            minIndex = j ;
            minKey = ( int )buffer[ j ] - 48 ;
          }
        }

        minTimestamp = timeStamp[ minKey ] ;

        for ( int j = 0 ; j < pageFrame ; j++ ) {  // if counter as the same , check time stamp smallest
          if ( counter[ minKey ] == counter[ ( int )buffer[ j ] - 48] && timeStamp[ ( int )buffer[ j ] - 48 ] < minTimestamp && j != minIndex ) {
            minTimestamp = timeStamp[ ( int )buffer[ j ] - 48 ] ;
            minIndex = j ;
            minKey = ( int )buffer[ j ] - 48 ;
          }
        }

        timeStamp[ minKey ] = 0 ;
        counter[ minKey ] = 0 ;
        inBuffer[ minKey ] = false ;

        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
        timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;


        for ( int j = minIndex ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
      else {  // 1. add new
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;
        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;

        if ( counter[ ( int )pageSchedule[ i ] - 48 ] == 1 ) {
          timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        }

        for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' ) {
        outfile << buffer[ j ] ;
      }
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl ;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl ;
}

void MFU_LRU( int pageFrame, string pageSchedule, fstream & outfile ) {  // Most Recently Used + Least Recently Used
  vector< char > buffer ;
  int timeStamp[100], counter[100] ;
  bool inBuffer[100] ;
  int pageFault = 0 ;

  for ( int i = 0 ; i < pageFrame ; i++ ) {  // initialize
    char temp = '-' ;
    buffer.push_back( temp ) ;
  }

  for ( int i = 0 ; i < 100 ; i++ ) {
    timeStamp[ i ] = 0 ;
    counter[ i ] = 0 ;
    inBuffer[ i ] = false ;
  }

  outfile << "--------------Most Frequently Used LRU Page Replacement -----------------------" << endl ;

  for ( int i = 0 ; i < pageSchedule.size() ; i++ ) {
    outfile << pageSchedule[ i ] << "\t" ;

    int storedIndex = 0 ;
    bool isFault = false, stored = false, full = true ;

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] == pageSchedule[ i ] ) {
        storedIndex = j ;
        stored = true ;
      }
    }

    if ( stored ) {
      counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
      timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;

      for ( int j = storedIndex ; j > 0 ; j-- ) {
        buffer[ j ] = buffer[ j - 1 ] ;
      }

      buffer[ 0 ] = pageSchedule[ i ] ;
    }
    else {
      isFault = true ;
      pageFault++ ;

      for ( int j = 0 ; j < pageFrame ; j++ ) {
        if ( buffer[ j ] == '-' ) {
          full = false ;
          break ;
        }
      }

      if ( full ) {
        int maxIndex = 0, maxKey = 0, maxCounter = -999999999, maxTimestamp ;

        for ( int j = pageFrame - 1 ; j >= 0 ; j-- ) {  // check counter smallest
          if ( maxCounter < counter[ ( int )buffer[ j ] - 48 ] ) {
            maxCounter = counter[ ( int )buffer[ j ] - 48 ] ;
            maxIndex = j ;
            maxKey = ( int )buffer[ j ] - 48 ;
          }
        }

        maxTimestamp = timeStamp[ maxKey ] ;

        for ( int j = 0 ; j < pageFrame ; j++ ) {  // if counter as the same , check time stamp smallest
          if ( counter[ maxKey ] == counter[ ( int )buffer[ j ] - 48 ] && timeStamp[ ( int )buffer[ j ] - 48 ] < maxTimestamp && j != maxIndex ) {
            maxTimestamp = timeStamp[ ( int )buffer[ j ] - 48 ] ;
            maxIndex = j ;
            maxKey = ( int )buffer[ j ] - 48 ;
          }
        }

        timeStamp[ maxKey ] = 0 ;
        counter[ maxKey ] = 0 ;
        inBuffer[ maxKey ] = false ;

        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;
        timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;

        for ( int j = maxIndex ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
      else {
        inBuffer[ ( int )pageSchedule[ i ] - 48 ] = true ;
        counter[ ( int )pageSchedule[ i ] - 48 ] += 1 ;\

        if ( counter[ ( int )pageSchedule[ i ] - 48 ] == 1 ) {
          timeStamp[ ( int )pageSchedule[ i ] - 48 ] = i + 1 ;
        }

        for ( int j = pageFrame - 1 ; j > 0 ; j-- ) {
          buffer[ j ] = buffer[ j - 1 ] ;
        }

        buffer[ 0 ] = pageSchedule[ i ] ;
      }
    }

    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( buffer[ j ] != '-' ) {
        outfile << buffer[ j ] ;
      }
    }

    if ( isFault ) {
      outfile << "\t" ;
      outfile << "F" ;
    }

    outfile << endl;
  }

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageFault - pageFrame << "  Page Frames = " << pageFrame << endl;
}

int main() {
  string fileName, pageSchedule ;
  fstream inFile, outfile ;
  int pageFrame ;

  while ( 1 ) {
    cout << "Please enter the file name (e.g., input1.txt) : " ;
    cin >> fileName ;
    inFile.open( fileName.c_str(), ios :: in ) ;

    inFile >> pageFrame >> pageSchedule ;
    inFile.close() ;

    fileName = "out_" + fileName ;
    outfile.open( fileName.c_str(), ios :: out ) ;

    FIFO( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    LRU( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    LFU_FIFO( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    MFU_FIFO( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    LFU_LRU( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    MFU_LRU( pageFrame, pageSchedule, outfile ) ;
    outfile << endl ;

    outfile.close() ;
  }
}
