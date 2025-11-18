//
// Created by 27489 on 2025/10/28.
//

#include "../Result.h"

#include <iostream>
#include <ostream>
#include "../../constant/Comon.h"

// Result::Result(TokenType syn, const std::string &token, int sum) {
//     this->syn=syn; this->token=token; this->sum=sum;
// }
Result::Result(TokenType syn, const std::string &token) {
    this->syn=syn; this->token=token;
}

void Result::output() const {
    std::cout<<"{ TypeNum(syn): "<<this->syn<<" ,\tTokenLiteral(token): \""<<this->token<<"\" }"<<std::endl;
    //<<", \tTokenType: "<<tokenTypeNames[this->syn];
    // if (this->sum!=-1)
    // {
    //     std::cout<<", \tNum: "<<this->sum<<" }"<<std::endl;
    // }
    // else {
    //     std::cout<<" }"<<std::endl;
    // }
}
TokenType Result::getTokenType() const {
    return this->syn;
}


