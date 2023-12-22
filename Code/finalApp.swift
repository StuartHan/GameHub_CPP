//
//  finalApp.swift
//  final
//
//  Created by Zongchen LI on 12/4/23.
//

import SwiftUI

@main
struct finalApp: App {
    var body: some Scene {
        DocumentGroup(newDocument: finalDocument()) { file in
            ContentView(document: file.$document)
        }
    }
}
