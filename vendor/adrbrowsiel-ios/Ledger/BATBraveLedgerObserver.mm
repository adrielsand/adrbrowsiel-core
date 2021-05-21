/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>
#import "BATadrbrowsielLedgerObserver.h"
#import "BATadrbrowsielLedger.h"

@interface BATadrbrowsielLedgerObserver ()
@property (nonatomic, weak) BATadrbrowsielLedger *ledger;
@end

@implementation BATadrbrowsielLedgerObserver

- (instancetype)initWithLedger:(BATadrbrowsielLedger *)ledger {
  if ((self = [super init])) {
    self.ledger = ledger;
  }
  return self;
}

@end
