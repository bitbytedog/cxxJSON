/*
 * COPYRIGHT NOTICE:
 *
 * (c) 2015 Stephen Williams. All rights reserved.
 *
 * This file is the property of Stephen Williams (the AUTHOR)
 *
 * This file contains proprietary information of the AUTHOR. No part of this
 * software may be reproduced, stored, transmitted, disclosed or used in any
 * form or by any means other than as expressly provided by in the written Software
 * License Agreement between the AUTHOR and the LICENSEE.
 *
 * The AUTHOR warrants for a period, as provided by the written Software
 * License Agreement between the AUTHOR and the licensee, that this software
 * will perform substantially to the AUTHORS specifications as published at the
 * time of shipment, exclusive of any updates or upgrades, and that the media
 * used for delivery of this software will be free from defects in material and
 * workmanship. The AUTHOR also warrants that he has the authority to enter into
 * and perform under the Software License Agreement and he is the copyright owner
 * of the software as originally delivered to his LICENSEE 
 *
 * THE AUTHOR MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
 * WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
 * A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
 * MATERIALS.
 *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY INDIRECT, SPECIAL,
 * CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
 * OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
 * ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
 * LIABILITY, OR OTHERWISE, AND WHETHER OR NOT HE HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Product names mentioned in this product that are trademarks or registered
 * trademarks of their respective companies are hereby acknowledged.
 *
 * Stephen Williams
 * Ivy Cottage, West Hatch
 * Taunton, Somerset,
 * UK, TA3 5RS.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <JSON/Object.h>

int main (int argc, char* argv[])
{
    std::ifstream is;

    is.open(argv[1], std::ifstream::in);

    if(is.is_open())
    {
        JSON::Object oids(is);
        std::cout << oids << std::endl;
    }
    else
    {
        std::cerr << "unable to open file: " << argv[1] << std::endl;
    }
    std::exit(0);
}
