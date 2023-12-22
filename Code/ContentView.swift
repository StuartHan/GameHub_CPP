//
//  ContentView.swift
//  final
//
//  Created by Zongchen LI on 12/4/23.
//

import SwiftUI

struct ContentView: View {
    @Binding var document: finalDocument

    var body: some View {
        TextEditor(text: $document.text)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView(document: .constant(finalDocument()))
    }
}
